#include "Global.h"
#include "PokerEngine.h"
#include "PreflopEvaluator.h"

#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <string.h>

PokerEngine::PokerEngine(void)
{
	preflop_evaluator = new PreflopEvaluator();
	table_context = new TableContext();

	// Provide initial seed for our random number generator
	srand((unsigned)time(0));
}

PokerEngine::~PokerEngine(void)
{
	if (preflop_evaluator) { delete preflop_evaluator; preflop_evaluator = 0; }
	if (table_context) { delete table_context; table_context = 0; }
}

//
// Determine the pre-flop action level
//
int PokerEngine::getPreflopAction()
{
	ProbabilityTriple ptriple;
    double random; 

	// Get our probability triple P(f,c,r) given our current table context
	ptriple = preflop_evaluator->GetPreflopAction(table_context);

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

//
// Update all our table context variables (OpenHoldem Version)
//
int PokerEngine::updateTableContext(pfgws_t pget_winholdem_symbol, holdem_state* state)
{
	bool iserr;
	char str[16];
	_cprintf("PokerEngine::updateTableContext(pfgws_t* pget_winholdem_symbol, holdem_state* state)");

	// Get the bot chair number and use that when calling the symbol accessor
	// NOTE: seems pointless.  May be deprecated in OpenHoldem
	int mychair = (int) (*pget_winholdem_symbol)(0,"userchair",iserr);
	_cprintf("PokerEngine::updateTableContext(1)");
	
	table_context->common_pot = (*pget_winholdem_symbol)(mychair,"potcommon",iserr);
	table_context->bot_chair = mychair;
	table_context->dealer_chair = (*pget_winholdem_symbol)(mychair,"dealerchair",iserr);
	_cprintf("PokerEngine::updateTableContext(2)");

	table_context->bot_cards[0] = state->m_cards[0];
	table_context->bot_cards[1] = state->m_cards[1];
	_cprintf("card[0] = 0x%08X card[1] = 0x%08X\n", state->m_cards[0], state->m_cards[1], 0xFFFFFFFF);


	table_context->betting_round = (*pget_winholdem_symbol)(mychair,"betround",iserr);

	table_context->bets_to_call = (*pget_winholdem_symbol)(mychair,"nbetstocall",iserr);
	table_context->bets_to_raise = (*pget_winholdem_symbol)(mychair,"nbetstorais",iserr);
	table_context->current_bet = (*pget_winholdem_symbol)(mychair,"ncurrentbets",iserr);

	// TODO: add safety check for numplayers
	for (int i=0; i < 10; i++)
	{
		memcpy(table_context->player[i].name, state->m_player[i].m_name, 16);

		table_context->player[i].cards[0] = state->m_player[i].m_cards[0];
		table_context->player[i].cards[1] = state->m_player[i].m_cards[1];

		sprintf_s(str, 16, "balance%u", (int)i);
		table_context->player[i].balance = (*pget_winholdem_symbol)(mychair,str,iserr);

		sprintf_s(str, 16, "currentbet%u", (int)i);
		table_context->player[i].current_bet = (*pget_winholdem_symbol)(mychair,str,iserr);

	}

	//TODO:  DEBUG THIS

	return 0;
}
