#include "LowLimitPreflopAnalyzer.h"
#include "Debug.h"
#include "Hand.h"

LowLimitPreflopAnalyzer::LowLimitPreflopAnalyzer(void)
{
}

LowLimitPreflopAnalyzer::~LowLimitPreflopAnalyzer(void)
{
}

/*
	IsHandInList(unsigned char c[2], const char* card_list)

	See if a given hand is in a list of hands
	card_list must be formatted as follows:
	"AA AKs 77 72 72s QQ KQs"
*/

// Returns a probability triple for the desired preflop action
ProbabilityTriple LowLimitPreflopAnalyzer::GetPreflopAction(TableInformation* table)
{
	Hand bot_cards;
	ProbabilityTriple	ptriple;

	Debug::log(LTRACE) << "LowLimitPreflopAnalyzer::GetPreflopAction(TableInformation* table)" << std::endl;

	TableContext* context = table->GetCurrentTableContext();

	// Capture our hole cards
	bot_cards = (context->bot_cards);

	//---------------------------------------------------------
	// Now we implement our expert system for pre-flop action.
	// Ideally, it should be written in a way that the more
	// general rules are evaluated earlier in the function.
	// NOTE(dbc): This expert is currently not correctly implemented 
	// --------------------------------------------------------

	if (IsHandInList(bot_cards, "AA KK"))
	{
		SET_PTRIPLE(ptriple, 0, .1, .9);  // Raise these premium hands in all situations
		return ptriple;
	}

	if (IsHandInList(bot_cards, "AKs QQ JJ"))
	{
		// Raise these premium hands only if less than three bets to call
		if (context->bets_to_call >= 3) 
			{ SET_PTRIPLE(ptriple, 1, 0, 0); return ptriple; }
		else
			{ SET_PTRIPLE(ptriple, 0, .1, .9); return ptriple; }
	}

	if (IsHandInList(bot_cards, "AQs AJs ATs"))
	{
		// Raise these hands if: 
		if(
			(context->current_bets == 1) &&		// no raise yet
			(context->bets_to_call == 1) &&		// one bet to call	
			(context->bot_bet_position <= 0) &&	// we are opening the betting or second to act
			(context->num_players_behind <= 2))	// in late position
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// Limp with these hands:
		if(
			(context->current_bets == 1) &&		// no raise yet
			(context->bets_to_call == 1) &&		// one bet to call	
			(context->bot_bet_position <= 0) &&	// we are opening the betting or second to act
			(context->num_players_playing <= 2))// in early position
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		if(
			(context->bets_to_call == 2) &&		// two bets to call
			(context->num_players_behind <= 2))	// we are in late position
		{ SET_PTRIPLE(ptriple, .5, .5, 0); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (IsHandInList(bot_cards, "AQ AJ AT"))
	{
		// Raise these hands if: 
		if(
			(context->current_bets == 1) &&		// no raise yet
			(context->bets_to_call == 1) &&		// one bet to call	
			(context->bot_bet_position <= 0) &&	// we are opening the betting or second to act
			(context->num_players_behind <= 2))	// in late position
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		if(
			(context->bets_to_call == 1) &&		// one bets to call
			(context->num_players_behind <= 2))	// we are in late position
		{ SET_PTRIPLE(ptriple, 0, .9, 1); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (IsHandInList(bot_cards, "AQ KQ"))
	{
		// Raise these hands if: 
		if(
			(context->current_bets == 1))		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// Limp with these hands:
		if(
			(context->current_bets == 1))		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		if(
			(context->bets_to_call == 2))		// two bets to call
		{ SET_PTRIPLE(ptriple, .5, .5, 0); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (IsHandInList(bot_cards, "TT 99 88 77 66"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1))		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (IsHandInList(bot_cards, "55 44 33 22"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1))		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (IsHandInList(bot_cards, "JTs T9s 98s"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1))		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (IsHandInList(bot_cards, "JT T9 98"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1))		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (IsHandInList(bot_cards, "JT T9 98"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1))		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	//-------------------------------------------------------
	// If we get to this point our expert system is missing
	// a scenario for a given hole card.  Ideally, we will
	// DEBUG_TRACE this event and somehow alert the user
	// so that it can be corrected.  For now, we'll just
	// fold to avoid more incorrect play
	//-------------------------------------------------------
	
	SET_PTRIPLE(ptriple, 1.0, 0, 0);		// always fold

	return ptriple;
}
