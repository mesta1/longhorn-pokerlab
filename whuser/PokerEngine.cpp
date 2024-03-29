#include "Global.h"
#include "Debug.h"
#include "PokerEngine.h"
#include "LowLimitPreflopAnalyzer.h"
#include "LowLimitPostflopAnalyzer.h"
#include "OpponentModel.h"

#include <conio.h>
#include <stdio.h>
#include <string.h>

PokerEngine::PokerEngine(void)
{
	// Here we can decide which analyzer class will be the default
	// evaluator.  For now, just use the default ones in development
	preflop_analyzer = new LowLimitPreflopAnalyzer();
	postflop_analyzer = new LowLimitPostflopAnalyzer();

	for (int i=0; i<10; i++) player[i] = new OpponentModel();
	
}

PokerEngine::~PokerEngine(void)
{
	if (preflop_analyzer) { delete preflop_analyzer; preflop_analyzer = 0; }
	if (postflop_analyzer) { delete postflop_analyzer; postflop_analyzer = 0; }
	for (int i=0; i<10; i++) { if (player[i]) { delete player[i]; player[i] = 0; } }
}

////////////////////////////////////////////////////////////////////////////
// Determine the pre-flop action level
//
int PokerEngine::getPreflopAction()
{
	ProbabilityTriple ptriple;
    double random; 

	// Get our probability triple P(f,c,r) given our current table context
	ptriple = preflop_analyzer->GetPreflopAction(&table);

	// Generate a random number 0.00 - 1.00
	random = rand()/(RAND_MAX + 1.0); 

	// Determine where the random number falls within P(f,c,r)
	// and return with our desired action
	if (random < ptriple.fold)
		return FOLD;
	else if ((random >= ptriple.fold) && (random < (ptriple.checkcall+ptriple.fold)))
		return CHECKCALL;
	else
		return BETRAISE;
}

////////////////////////////////////////////////////////////////////////////
// Determine the  post-flop action level
//
int PokerEngine::getPostflopAction()
{
	return FOLD;
}

////////////////////////////////////////////////////////////////////////////
// This is a placeholder in case we decide to integrate pre-flop and 
// post-flop playint PokerEngine::getAction()
//
int PokerEngine::getAction()
{
	return FOLD;
}

////////////////////////////////////////////////////////////////////////////
// Update all our table context variables (OpenHoldem Version)
//
int PokerEngine::UpdateTableContext(pfgws_t pget_winholdem_symbol, holdem_state* state)
{
	bool			iserr;
	int				safe_to_update;
	char			str[16];
	TableContext	new_context;
	PlayerContext	player_context[10];
	int				playersplayingbits;

	Debug::log(LDEBUG4) << "PokerEngine::UpdateTableContext(pfgws_t* pget_winholdem_symbol, holdem_state* state)" << std::endl;

	/////////////////////////////////////////////////////////////
	// First, pull all the information on the new table context
	//
	int mychair = (int) (*pget_winholdem_symbol)(0,"userchair",iserr);	// Get the bot chair number and use that when calling the symbol accessor
																		// NOTE: seems pointless.  May be deprecated in OpenHoldem?	

	state->m_title[63] = 0; // kludge
	strncpy_s(new_context.table_name, 64, state->m_title, 64);
	new_context.hand_number = (*pget_winholdem_symbol)(mychair,"handnumber",iserr);

	new_context.common_pot = (*pget_winholdem_symbol)(mychair,"potcommon",iserr);
	new_context.total_pot = (*pget_winholdem_symbol)(mychair,"pot",iserr);

	new_context.bot_chair = mychair;
	new_context.dealer_chair = (int)(*pget_winholdem_symbol)(mychair,"dealerchair",iserr);

	new_context.bot_cards.SetCard(0, Card(state->m_player[mychair].m_cards[0]));
	new_context.bot_cards.SetCard(1, Card(state->m_player[mychair].m_cards[1]));

	new_context.common_cards.flop1 = state->m_cards[0];
	new_context.common_cards.flop2 = state->m_cards[1];
	new_context.common_cards.flop3 = state->m_cards[2];
	new_context.common_cards.turn = state->m_cards[3];
	new_context.common_cards.river = state->m_cards[4];

	new_context.betting_round = (int)(*pget_winholdem_symbol)(mychair,"betround",iserr);

	new_context.bets_to_call = (*pget_winholdem_symbol)(mychair,"nbetstocall",iserr);
	new_context.bets_to_raise = (*pget_winholdem_symbol)(mychair,"nbetstorais",iserr);
	new_context.current_bets = (*pget_winholdem_symbol)(mychair,"ncurrentbets",iserr);

	new_context.bot_deal_position = (int) (*pget_winholdem_symbol)(mychair,"dealposition",iserr);
	new_context.bot_bet_position = (int) (*pget_winholdem_symbol)(mychair,"betposition",iserr);

	new_context.num_players_dealt = (int) (*pget_winholdem_symbol)(mychair,"nplayersdealt",iserr);
	new_context.num_players_playing = (int) (*pget_winholdem_symbol)(mychair,"nplayersplaying",iserr);
	new_context.num_players_behind = new_context.num_players_playing - new_context.bot_bet_position;

	playersplayingbits = (int) (*pget_winholdem_symbol)(mychair,"playersplayingbits",iserr);

	// Iterate through all ten players and pull their information
	for (int i=0; i < 10; i++)
	{

		memcpy(player_context[i].name, state->m_player[i].m_name, 16);

		player_context[i].cards[0] = state->m_player[i].m_cards[0];
		player_context[i].cards[1] = state->m_player[i].m_cards[1];

		sprintf_s(str, sizeof(str), "balance%u", (int)i);
		player_context[i].balance = (*pget_winholdem_symbol)(mychair,str,iserr);

		sprintf_s(str, sizeof(str), "currentbet%u", (int)i);
		player_context[i].current_bet = (*pget_winholdem_symbol)(mychair,str,iserr);
		player_context[i].is_playing = ((playersplayingbits >> i)& 0x0001);
	}

	/////////////////////////////////////////////////////////////
	// Determine if anything has changed.  If not, then we can
	// skip out on the rest because this function might get
	// called numerous times before a change has happened.
	// NOTE: not sure we can determine "check" right now. 
	// TODO: investigate "check"
	
	if (table.HasTableContextChanged(new_context))
	{
		// Store the table context
		safe_to_update = table.UpdateTableContext(new_context);
		
		// Has our table class determined that this is an appropriate
		// change do perform calculations on?
		if (safe_to_update)
		{
			// Tell the opponent model about our new player
			// information
			for (int i=0; i < 10; i++)
			{
				//TODO:
				//if (PlayerActedSinceLastChange(i))
				// Update the opponent model if the player context has
				// changed.
				if (player[i]->HasPlayerContextChanged(player_context[i]))
					player[i]->UpdatePlayerContext(player_context[i]);
			}

			/////////////////////////////////////////////////////
			// TODO:  Here we change the analyzer to fit the
			// current flavor of poker for the given hand.  That
			// is, if we determine we are now heads up we could
			// switch to the heads-up analyzer which might have
			// a Nash-equilibrium optimized solution.
			// 
			// if (AreWeHeadsUp()) postflop_analyzer = p_HeadsUpLimitNashEqAnalyzer;

			// Finally, rerun statistical calculations with the latest 
			// information. We will be able to morph this into a
			// separate thread (or threads for multicore!) that
			// runs in parallel and does not slow down our UI or table
			// scraper.  Only run these calculations if our bot is ready.
			if (table.IsBotHandConfirmed())
			{
				if (new_context.betting_round == PREFLOP)
				{
					//preflop_analyzer->RunCalculations(&table, &player[0]);
				}
				else
				{
					postflop_analyzer->RunCalculations(&table, &player[0]);
				}
			}
		}

		// That's all for now.  We'll wait until our PokerEngine is
		// called again to determine what action to take.
	}

	return 0;
}

