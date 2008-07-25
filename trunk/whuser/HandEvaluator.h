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
	double	CalculateProbabilityOfWinning(TableInformation* table, OpponentModel* players);

protected:
	// See if a given hand is in a list of hands
	int isHandInList(unsigned char c[2], const char* card_list);

private:
	inline void DealCardsToOpponent(OpponentModel*, unsigned char*, unsigned char*);
	inline void DealFlop(unsigned char*, unsigned char*);
	inline void DealTurn(unsigned char*, unsigned char*);
	inline void DealRiver(unsigned char*, unsigned char*);

	inline int HandEvaluator::EvaluateBestSevenCardHand(unsigned char*, unsigned char*);
};

#endif