#pragma once

#include "inlines/eval.h"

class Hand
{
public:
	Hand(void);
	~Hand(void);
	
	int Evaluate(void);
	int AddCard(unsigned char);
	void Reset(void);
	void ToString(char*);

	//unsigned int &operator[] (int);

private:
	int			cards[7];   // cards are stored in poker-eval format
	HandVal		hand_value;
	int			m_size;
};

