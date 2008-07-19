#include "PreflopEvaluator.h"

#include <stdio.h>
#include <string.h>

PreflopEvaluator::PreflopEvaluator(void)
{
	// Normalize the preflop strength matrix
	int i = 0;
	double maxval = 0;
	for (i=0; i < 169; i++) { 
		if (preflophands_prwin[i] > maxval)
			maxval = preflophands_prwin[i];
	}
	for (i=0; i < 169; i++) { 
		preflophands_prwin[i] /= maxval;
	}

	// Initialize preflop thresholds with default values
	// TODO: Make more robust
	preflophands_threshold[FOLD] = 0;
	preflophands_threshold[MAKE1] = preflophands_prwin[39];
	preflophands_threshold[MAKE2] = preflophands_prwin[16];  // Raise / call two bets with 
	preflophands_threshold[MAKE3] = preflophands_prwin[5];  // Re-raise / call three bets with JJ or better
	preflophands_threshold[CAP] = preflophands_prwin[1];	// Cap with KK or better
}

PreflopEvaluator::~PreflopEvaluator(void)
{
}

/*
	ishandinlist(unsigned char c[2], const char* card_list)

	See if a given hand is in a list of hands
	card_list must be formatted as follows:
	"AA AKs 77 72 72s QQ KQs"
*/
int PreflopEvaluator::isHandInList(unsigned char c[2], const char* card_list)
{
	char strhand[4]; strhand[3] = NULL;
	char revstrhand[4]; revstrhand[3] = NULL;
	const char ranktochar[]="xA23456789TJQKA";

	// Convert hand from pokerval format to string
	strhand[0] = ranktochar[RANK(c[0])];
	strhand[1] = ranktochar[RANK(c[1])];
	if (RANK(c[0])==RANK(c[1]))
		strhand[2]=NULL;
	else
		ISSUITED(c[0],c[1]) ? strhand[2]='s' : strhand[2]=' ';
	
	// Make mirror image of string in case cards are in reverse order
	revstrhand[1] = strhand[0];
	revstrhand[0] = strhand[1];
	revstrhand[2] = strhand[2];

	if (strstr(card_list, strhand)) return 1; // found card in string list
	if (strstr(card_list, revstrhand)) return 1; // found card in string list
		
	return 0;  // found no match
}

// Returns a probability triple for the desired preflop action
ProbabilityTriple PreflopEvaluator::GetPreflopAction(TableContext* context)
{
	unsigned char cards[2];
	ProbabilityTriple	ptriple;

	// Capture our hole cards
	cards[0] = context->bot_cards[0];
	cards[1] = context->bot_cards[1];

	//---------------------------------------------------------
	// Now we implement our expert system for pre-flop action.
	// Ideally, it should be written in a way that the more
	// general rules are evaluated earlier in the function.
	// --------------------------------------------------------

	if (isHandInList(cards, "AA KK"))
	{
		SET_PTRIPLE(ptriple, 0, .1, .9);  // Raise these premium hands in all situations
		return ptriple;
	}

	if (isHandInList(cards, "AKs QQ JJ"))
	{
		// Raise these premium hands only if less than three bets to call
		if (context->bets_to_call >= 3) 
			{ SET_PTRIPLE(ptriple, 1, 0, 0); return ptriple; }
		else
			{ SET_PTRIPLE(ptriple, 0, .1, .9); return ptriple; }
	}

	if (isHandInList(cards, "AQs AJs ATs"))
	{
		// Raise these hands if: 
		if(
			(context->current_bets == 1)		// no raise yet
			(context->bets_to_call == 1) &&		// one bet to call	
			(context->bot_bet_position <= 0) &&	// we are opening the betting or second to act
			(context->num_players_behind <= 2))	// in late position
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// Limp with these hands:
		if(
			(context->current_bets == 1)		// no raise yet
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

	if (isHandInList(cards, "AQ AJ AT"))
	{
		// Raise these hands if: 
		if(
			(context->current_bets == 1)		// no raise yet
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

	if (isHandInList(cards, "AQ KQ"))
	{
		// Raise these hands if: 
		if(
			(context->current_bets == 1)		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// Limp with these hands:
		if(
			(context->current_bets == 1)		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		if(
			(context->bets_to_call == 2) &&		// two bets to call
		{ SET_PTRIPLE(ptriple, .5, .5, 0); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (isHandInList(cards, "TT 99 88 77 66"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1)		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (isHandInList(cards, "55 44 33 22"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1)		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (isHandInList(cards, "JTs T9s 98s"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1)		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (isHandInList(cards, "JT T9 98"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1)		// no raise yet
		{ SET_PTRIPLE(ptriple, 0, .2, .8); return ptriple; }

		// default action
		SET_PTRIPLE(ptriple, .1, .9, 0); return ptriple;
	}

	if (isHandInList(cards, "JT T9 98"))
	{
		// Limp with these hands:
		if(
			(context->current_bets == 1)		// no raise yet
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
