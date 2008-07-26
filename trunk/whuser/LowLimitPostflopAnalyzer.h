#pragma once
#include "HandAnalyzer.h"
#include "TableInformation.h"
#include "OpponentModel.h"

class LowLimitPostflopAnalyzer :
	public HandAnalyzer
{
public:
	LowLimitPostflopAnalyzer(void);
	~LowLimitPostflopAnalyzer(void);

	// Returns a probability triple for the desired preflop action
	ProbabilityTriple GetPreflopAction(TableInformation* table);


};
