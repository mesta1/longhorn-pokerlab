#pragma once

#ifndef _HANDANALZYER_H_
#define _HANDANALYZER_H_

#include "Global.h"
#include "TableInformation.h"
#include "OpponentModel.h"
#include "Hand.h"
#include "Card.h"

class HandAnalyzer
{
public:
	HandAnalyzer(void);
	~HandAnalyzer(void);

	ProbabilityTriple GetPreflopAction(TableInformation* table);
	ProbabilityTriple GetPostflopAction(TableInformation* table);
	void RunCalculations(TableInformation* table, OpponentModel** players);

protected:
	// See if a given hand is in a list of hands
	int IsHandInList(Hand h, const char* card_list);

private:
	inline void DealCardsToOpponent(OpponentModel*, Hand*, unsigned char*);
	inline void DealFlop(CommonCards*, unsigned char*);
	inline void DealTurn(CommonCards*, unsigned char*);
	inline void DealRiver(CommonCards*, unsigned char*);
	double	CalculateProbabilityOfWinning(TableInformation* table, OpponentModel** players);
	inline int EvaluateBestSevenCardHand(Hand*, const CommonCards&);

	double p_win;
	double p_lose;
	double p_tie;
};

#endif