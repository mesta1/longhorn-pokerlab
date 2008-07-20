#pragma once

#ifndef _PREFLOPEVALUATOR_H_
#define _PREFLOPEVALUATOR_H_

#include "Global.h"
#include "HandEvaluator.h"

class PreflopEvaluator : public HandEvaluator
{
public:
	PreflopEvaluator(void);
	~PreflopEvaluator(void);

	// Returns a probability triple for the desired preflop action
	ProbabilityTriple GetPreflopAction(TableContext* context);

private:

};

#endif