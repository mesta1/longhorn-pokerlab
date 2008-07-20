#pragma once

#ifndef _HANDEVALUATOR_H_
#define _HANDEVALUATOR_H_

#include "Global.h"
#include "TableInformation.h"
#include "OpponentModel.h"

class HandEvaluator
{
public:
	HandEvaluator(void);
	~HandEvaluator(void);

	ProbabilityTriple GetPreflopAction(TableInformation* table);
	ProbabilityTriple GetPostflopAction(TableInformation* table);

protected:
	// See if a given hand is in a list of hands
	int isHandInList(unsigned char c[2], const char* card_list);

};

#endif