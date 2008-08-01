#pragma once

#include "inlines/eval.h"
#include "Card.h"

class Hand
{
public:
	Hand(void);
	Hand(unsigned char&, unsigned char&);
	Hand(const Card&, const Card&);
	~Hand(void);
	
	unsigned int Evaluate(const CommonCards&);		// Evaluate best 7-card hand
	void SetCard(int, const Card&);
	void Reset(void);
	void HandValueToString(char*);
	int IsValid() const;

	const Card operator[](const int) const;

private:
	int	ConvertCardToPokerEvalCard(const Card& card) const;
	//int ConvertCardToPokerEvalCard(const unsigned char card) const;
	Card ConvertPokerEvalCardToCard(int card) const;

	int			pokereval_cards[2];		// cards stored in poker-eval format
	Card		cards[2];				// cards stored in Card class
	HandVal		hand_value; // last evaluated hand value (no reason to store this yet)
};

ostream& operator <<(ostream&, const Hand&);
