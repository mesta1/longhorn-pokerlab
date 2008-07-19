#pragma once

#ifndef _PREFLOPEVALUATOR_H_
#define _PREFLOPEVALUATOR_H_

#include "Global.h"

class PreflopEvaluator
{
public:
	PreflopEvaluator(void);
	~PreflopEvaluator(void);

	// Returns a probability triple for the desired preflop action
	ProbabilityTriple GetPreflopAction(TableContext* context);

private:

	// See if a given hand is in a list of hands
	int isHandInList(unsigned char c[2], const char* card_list);

/*
	int preflophands_threshold[5]
	This variable is an array of the minimum preflophands_prwin[] value fo
	each action below
*/
	enum ACTION	{
		FOLD = 0,
		MAKE1,		//	call one bet
		MAKE2,		//	call two bets / raise if unraised
		MAKE3,		//	call three bets / raise-if-reraised
		CAP			//	cap
	};

	double preflophands_threshold[5];

};

#endif