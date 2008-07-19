#include "HandEvaluator.h"

HandEvaluator::HandEvaluator(void)
{
}

HandEvaluator::~HandEvaluator(void)
{
}

ProbabilityTriple HandEvaluator::GetPreflopAction(TableContext* context)
{
	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}

ProbabilityTriple HandEvaluator::GetPostflopAction(TableContext* context)
{
	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}
