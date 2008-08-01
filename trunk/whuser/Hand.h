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

	int IsSuited() const;
	int IsPocketPair() const;
	int IsConnector() const;			// returns gap between cards (i.e., "8c 6h" returns 2)

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

enum POKEREVAL
{
    HiCard			= 0x00000001,
    OnePair			= 0x01000000,
    TwoPair			= 0x02000000,
    ThreeOfAKind	= 0x04000000,
    Straight		= 0x08000000,
    Flush			= 0x10000000,
    FullHouse		= 0x20000000,
    FourOfAKind		= 0x40000000,
    StraightFlush	= 0x80000000,
    RoyalFlush		= 0x800EDCBA,
    FiveOfAKind		= 0xFF000000
};