#pragma once

#ifndef _pokerengine_h_
#define _pokerengine_h_

#include "Global.h"
#include "PreflopEvaluator.h"
#include "OpponentModel.h"
#include "TableInformation.h"

#include <stdlib.h>


class PokerEngine
{
public:
	PokerEngine(void);
	~PokerEngine(void);

	// Determine the pre-flop action level
	int getPreflopAction(void);
	int getPostflopAction(void);
	int getAction(void);

private:
	HandEvaluator*		preflop_evaluator;
	HandEvaluator*		postflop_evaluator;

	TableInformation*	table;
	OpponentModel*		player[10];

public:
	// Update all our table context variables (OpenHoldem Version)
	int updateTableContext(pfgws_t, holdem_state*);
};



#endif