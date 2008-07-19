#pragma once

#ifndef _HANDEVALUATOR_H_
#define _HANDEVALUATOR_H_

#include "Global.h"

class HandEvaluator
{
public:
	HandEvaluator(void);
	~HandEvaluator(void);

	ProbabilityTriple GetPreflopAction(TableContext* context);
	ProbabilityTriple GetPostflopAction(TableContext* context);
};

#endif