#include "Global.h"
#include "WeightedHandTable.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

WeightedHandTable::WeightedHandTable(void)
{
	// Initialize weight table to make all cards fully
	// playable.
	for (int i=0; i<1326; i++) hand_table[i] = 0;

	// Initialize total sum of weights
	//total_weight = 1326;

	for (int i=1320; i<1326; i++) hand_table[i] = 1;
	for (int i=1122; i<1127; i++) hand_table[i] = 1;
	total_weight = 12;
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
	const int PREINDEX[13] = { 0, 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66};
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

	// store the new weight
	hand_table[index] = weight;

	// Now, add up all the weight values and save the sum
	// which we will use when we want to deal a random hand
	total_weight = 0;
	for (int i=0; i < 1326; i++)
	{
		total_weight += hand_table[i];
	}

}

double WeightedHandTable::GetWeight(unsigned char card1, unsigned char card2)
{
	int index;

	// determine the appropriate index for these hole cards
	index = GetIndex(card1, card2);

	return hand_table[index];
}

#define DBL_EPSILON  2.2204460492503131e-016D
#define DBL_EQ(X,Y) ( (X - Y) < DBL_EPSILON )	// X == Y
#define DBL_GT(X,Y) ( (X - Y) > DBL_EPSILON )	// X > Y */
#define DBL_LT(X,Y) ( (Y - X) > DBL_EPSILON )	// Y > X */

void WeightedHandTable::DealRandomHand(unsigned char* cards, unsigned char* card_mask)
{
	double random = 0, sum = 0;
	int index;
	
	// Generate random number from 0 to the sum total weight
	random = ((total_weight)*rand())/(RAND_MAX + 1.0);

	// Run through the weight table and stop once we reach the
	// random number.
	for (index=0; index < 1326; index++)
	{
		sum += hand_table[index];
		if (random < sum) break;
	}

	cards[0] = _tabletohand[index*2];
	cards[1] = _tabletohand[index*2+1];

	return;
}