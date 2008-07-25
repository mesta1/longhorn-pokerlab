#include "Global.h"
#include "WeightedHandTable.h"

#include <memory.h>

WeightedHandTable::WeightedHandTable(void)
{
	// Initialize weight table to make all cards fully
	// playable.
	for (int i=0; i<1326; i++) hand_table[i] = (double) 1.0;
}

WeightedHandTable::~WeightedHandTable(void)
{
}

int WeightedHandTable::GetIndex(unsigned char card1, unsigned char card2)
{
	int index = 0;

	// Note that this function depends on RANK(card) returning 2 for 2 and 14 for A

	// Make sure cards are ordered by rank (i.e., AhQh not QhAh)
	if (RANK(card1) < RANK(card2))
	{ 
		unsigned char t=card1; card1=card2; card2=t;
	} 
	else if (RANK(card1) == RANK(card2))
	{
		// Make sure cards are ordered by suit (i.e., AcAh not AhAc)
		// CLUBS < DIAMONDS < HEARTS < SPADES
		if (SUIT(card1) > SUIT(card2))
		{ 
			unsigned char t=card1; card1=card2; card2=t; 
		}
	}

	// Increase offset based on the rank of first card	
	// For every high card there are 6 combinations of pocket pairs
	// and 16 different combinations per card rank above the
	// first card)
					  // 2  3  4  5  6  7   8   9   T   J   Q   K   A
	int PREINDEX[13] = { 0, 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66};
	index += ((RANK(card1)-2) * 6)	+			// skip pocket pairs
			 (PREINDEX[RANK(card1)-2] * 16);	// skip lower non-pocket pairs

	// Determine our offset for pocket pairs
	if (RANK(card1) == RANK(card2))
	{
		switch (SUIT(card1))
		{
		case CLUBS:
			if (SUIT(card2)==DIAMONDS) index += 0;
			if (SUIT(card2)==HEARTS) index += 1;		
			if (SUIT(card2)==SPADES) index += 2;
			break;
		case DIAMONDS:
			if (SUIT(card2)==HEARTS) index += 3;		
			if (SUIT(card2)==SPADES) index += 4;
			break;
		case HEARTS:
			if (SUIT(card2)==SPADES) index += 5;
			break;
		}
	} 
	// Determine our offset for for non-pocket pairs
	else
	{
		index += (RANK(card2)-2) * 16;	// increase offset beyond cards ranked lower than our second card

		// increase offset based upon suit of first card		
		switch (SUIT(card1))
		{
		case HEARTS: index += 0; break;
		case DIAMONDS: index += 4; break;
		case CLUBS: index += 8; break;
		case SPADES: index += 12; break;
		}

		// increase offset to the correct suit for second card		
		switch (SUIT(card2))
		{
		case HEARTS: index += 0; break;
		case DIAMONDS: index += 1; break;
		case CLUBS: index += 2; break;
		case SPADES: index += 3; break;
		}
	}

	return index;
}

void WeightedHandTable::SetWeight(unsigned char card1, unsigned char card2, double weight)
{
	int index;

	// determine the appropriate index for these hole cards
	index = GetIndex(card1, card2);

	hand_table[index] = weight;
}

double WeightedHandTable::GetWeight(unsigned char card1, unsigned char card2)
{
	int index;

	// determine the appropriate index for these hole cards
	index = GetIndex(card1, card2);

	return hand_table[index];
}
