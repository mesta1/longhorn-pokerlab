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

};

#endif