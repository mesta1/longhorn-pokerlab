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
	
	int Evaluate(const CommonCards&);
	void SetCard(int, const Card&);
	void Reset(void);
	void ToString(char*);

	Card operator[](const int) const;

private:
	int	ToPokerEvalCard(const Card& card) const;
	int ToPokerEvalCard(const unsigned char card) const;
	Card ToCard(int card) const;

	int			cards[2];   // cards are stored in poker-eval format
	HandVal		hand_value; // last evaluated hand value (no reason to store this yet)
	int			m_size;		// UNUSED
};

ostream& operator <<(ostream&, const Hand&);
