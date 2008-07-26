#include "Global.h"
#include "Debug.h"
#include "PokerEngine.h"
#include "LowLimitPreflopAnalyzer.h"
#include "LowLimitPostflopAnalyzer.h"
#include "OpponentModel.h"

#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <string.h>

PokerEngine::PokerEngine(void)
{
	// Here we can decide which analyzer class will be the default
	// evaluator.  For now, just use the default ones in development
	preflop_analyzer = new LowLimitPreflopAnalyzer();
	postflop_analyzer = new LowLimitPostflopAnalyzer();
	
	table = new TableInformation();

	for (int i=0; i<10; i++) player[i] = new OpponentModel();

	// Provide initial seed for our random number generator
	srand((unsigned)time(0));
}

PokerEngine::~PokerEngine(void)
{
	if (preflop_analyzer) { delete preflop_analyzer; preflop_analyzer = 0; }
	if (postflop_analyzer) { delete postflop_analyzer; postflop_analyzer = 0; }
	if (table) { delete table; table = 0; }
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
	ptriple = preflop_analyzer->GetPreflopAction(table);

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
int PokerEngine::updateTableContext(pfgws_t pget_winholdem_symbol, holdem_state* state)
{
	Debug::log(LTRACE) << "PokerEngine::updateTableContext(pfgws_t* pget_winholdem_symbol, holdem_state* state)" << std::endl;

	bool iserr;
	char str[16];
	TableContext new_context;
	
	/////////////////////////////////////////////////////////////
	// First, pull all the information on the new table context
	//
	int mychair = (int) (*pget_winholdem_symbol)(0,"userchair",iserr);	// Get the bot chair number and use that when calling the symbol accessor
																		// NOTE: seems pointless.  May be deprecated in OpenHoldem	
	new_context.common_pot = (*pget_winholdem_symbol)(mychair,"potcommon",iserr);
	new_context.bot_chair = mychair;
	new_context.dealer_chair = (*pget_winholdem_symbol)(mychair,"dealerchair",iserr);

	new_context.bot_cards[0] = state->m_cards[0];
	new_context.bot_cards[1] = state->m_cards[1];

	new_context.betting_round = (*pget_winholdem_symbol)(mychair,"betround",iserr);

	new_context.bets_to_call = (*pget_winholdem_symbol)(mychair,"nbetstocall",iserr);
	new_context.bets_to_raise = (*pget_winholdem_symbol)(mychair,"nbetstorais",iserr);
	new_context.current_bets = (*pget_winholdem_symbol)(mychair,"ncurrentbets",iserr);

	PlayerContext player_context[10];
	for (int i=0; i < 10; i++)		// Iterate through all ten players and pull their information
	{

		memcpy(player_context[i].name, state->m_player[i].m_name, 16);

		player_context[i].cards[0] = state->m_player[i].m_cards[0];
		player_context[i].cards[1] = state->m_player[i].m_cards[1];

		sprintf_s(str, sizeof(str), "balance%u", (int)i);
		player_context[i].balance = (*pget_winholdem_symbol)(mychair,str,iserr);

		sprintf_s(str, sizeof(str), "currentbet%u", (int)i);
		player_context[i].current_bet = (*pget_winholdem_symbol)(mychair,str,iserr);

	}

	/////////////////////////////////////////////////////////////
	// Determine if anything has changed.  If not, then we can
	// skip out on the rest because this function might get
	// called numerous times before a change has happened.
	// NOTE: not sure we can determine "check" right now. 
	// TODO: investigate "check"
	
	//if (HasTableContextChanged(&new_context))
	//{
		// Store the table context 
		table->UpdateTableContext(new_context);

		// Now tell the opponent model about our new player
		for (int i=0; i < 10; i++)
		{
			player[i]->UpdatePlayerContext(player_context[i]);
		}

		/////////////////////////////////////////////////////
		// TODO:  Here we change the analyzer to fit the
		// current flavor of poker for the given hand.  That
		// is, if we determine we are now heads up we could
		// switch to the heads-up analyzer which might have
		// a Nash-equilibrium optimized solution.
		//
		// postflop_analyzer = p_HeadsUpLimitNashEqAnalyzer;

		//postflop_analyzer->
	//}

	return 0;
}
