#pragma once

#ifndef _LOWLIMITPREFLOPANALZYER_H_
#define _LOWLIMITPREFLOPANALYZER_H_

#include "Global.h"
#include "HandAnalyzer.h"
#include "TableInformation.h"
#include "OpponentModel.h"

class LowLimitPreflopAnalyzer : public HandAnalyzer
{
public:
	LowLimitPreflopAnalyzer(void);
	~LowLimitPreflopAnalyzer(void);

	// Returns a probability triple for the desired preflop action
	ProbabilityTriple GetPreflopAction(TableInformation* table);

private:

};

#endif