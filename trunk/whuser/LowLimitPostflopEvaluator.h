#pragma once
#include "handevaluator.h"

class LowLimitPostflopEvaluator :
	public HandEvaluator
{
public:
	LowLimitPostflopEvaluator(void);
	~LowLimitPostflopEvaluator(void);

	// Returns a probability triple for the desired preflop action
	ProbabilityTriple GetPreflopAction(TableContext* context);


};
