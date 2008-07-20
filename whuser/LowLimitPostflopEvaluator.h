#pragma once
#include "HandEvaluator.h"
#include "TableInformation.h"
#include "OpponentModel.h"

class LowLimitPostflopEvaluator :
	public HandEvaluator
{
public:
	LowLimitPostflopEvaluator(void);
	~LowLimitPostflopEvaluator(void);

	// Returns a probability triple for the desired preflop action
	ProbabilityTriple GetPreflopAction(TableInformation* table);


};
