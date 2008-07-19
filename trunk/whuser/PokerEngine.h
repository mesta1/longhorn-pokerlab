#pragma once

#ifndef _pokerengine_h_
#define _pokerengine_h_

#include <stdlib.h>

#include "PreflopEvaluator.h"
#include "Global.h"

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
	TableContext*		table_context;
public:
	// Update all our table context variables (OpenHoldem Version)
	int updateTableContext(pfgws_t, holdem_state*);
};



#endif