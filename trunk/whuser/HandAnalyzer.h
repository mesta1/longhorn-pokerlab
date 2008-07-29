#pragma once

#ifndef _HANDANALZYER_H_
#define _HANDANALYZER_H_

#include "Global.h"
#include "TableInformation.h"
#include "OpponentModel.h"

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
	int IsHandInList(unsigned char c[2], const char* card_list);

private:
	inline void DealCardsToOpponent(OpponentModel*, unsigned char*, unsigned char*);
	inline void DealFlop(unsigned char*, unsigned char*);
	inline void DealTurn(unsigned char*, unsigned char*);
	inline void DealRiver(unsigned char*, unsigned char*);
	double	CalculateProbabilityOfWinning(TableInformation* table, OpponentModel** players);
	inline int EvaluateBestSevenCardHand(unsigned char*, unsigned char*);

	double p_win;
	double p_lose;
	double p_tie;
};

#endif