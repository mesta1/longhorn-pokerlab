#include "HandDistribution.h"
#include "Hand.h"
#include "Card.h"

HandDistribution::HandDistribution(void)
{
	// Call the base class constructor to create a weighted table
	// with 1326 elements -- one for each unique two-card hand.
	// The initial weights of the hands will all be 1.0 (i.e.,
	// each hand is equally likely to be held)

	WeightedDistribution(1326);
}

HandDistribution::~HandDistribution(void)
{
	// NOTE: the base class deconstructor is automatically 
	// called for cleanup
}

void HandDistribution::DealRandomHand(Hand* hand)
{

    double random = 0, sum = 0;
    int index;
    
    // Generate random number from 0 to 1326 using
	// the weights for each element
	index = WeightedDistribution::rand();

	// Now we have a valid card. Save and return.
    hand->SetCard(0, Card(_tabletohand[index*2]));
    hand->SetCard(1, Card(_tabletohand[index*2+1]));

	if (hand->IsValid())
		return;
	else
	{
		// ERROR
		return;
	}
}

int HandDistribution::GetHandIndex(const Hand& hand)
{
	int index = 0;
	Card	card1, card2;

	card1 = hand[0];
	card2 = hand[1];

	// Note that this function depends on card.Rank() returning 2 for 2 and 14 for A

	if (card1.Rank() < card2.Rank())
	{ 
		// Make sure cards are ordered by rank (i.e., AhQh not QhAh)
		// Swap if they are not
		Card t = card1; card1 = card2; card2 = t;
	} 
	else if (card1.Rank() == card2.Rank())
	{
		// Make sure cards are ordered by suit (i.e., AcAh not AhAc)
		// CLUBS < DIAMONDS < HEARTS < SPADES
		if (card1.Suit() > card2.Suit())
		{ 
			// Swap if they are not
			Card t = card1; card1 = card2; card2 = t;
		}
	}

	// Increase offset based on the rank of first card	
	// For every high card there are 6 combinations of pocket pairs
	// and 16 different combinations per card rank above the
	// first card)
							// 2  3  4  5  6  7   8   9   T   J   Q   K   A
	const int PREINDEX[13] = { 0, 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66};
	index += ((card1.Rank()-2) * 6)	+			// skip pocket pairs
			 (PREINDEX[card1.Rank()-2] * 16);	// skip lower non-pocket pairs

	// Determine our offset for pocket pairs
	if (card1.Rank() == card2.Rank())
	{
		switch (card1.Suit())
		{
		case Card::CLUBS:
			if (card2.Suit() == Card::DIAMONDS) index += 0;
			if (card2.Suit() == Card::HEARTS) index += 1;		
			if (card2.Suit() == Card::SPADES) index += 2;
			break;
		case Card::DIAMONDS:
			if (card2.Suit() == Card::HEARTS) index += 3;		
			if (card2.Suit() == Card::SPADES) index += 4;
			break;
		case Card::HEARTS:
			if (card2.Suit() == Card::SPADES) index += 5;
			break;
		}
	} 
	// Determine our offset for for non-pocket pairs
	else
	{
		index += (card2.Rank()-2) * 16;	// increase offset beyond cards ranked lower than our second card

		// increase offset based upon suit of first card		
		switch (card2.Suit())
		{
		case Card::HEARTS: index += 0; break;
		case Card::DIAMONDS: index += 4; break;
		case Card::CLUBS: index += 8; break;
		case Card::SPADES: index += 12; break;
		}

		// increase offset to the correct suit for second card		
		switch (card2.Suit())
		{
		case Card::HEARTS: index += 0; break;
		case Card::DIAMONDS: index += 1; break;
		case Card::CLUBS: index += 2; break;
		case Card::SPADES: index += 3; break;
		}
	}

	return index;
}

void HandDistribution::SetWeight(const Hand& hand, double weight)
{
	int index;

	// determine the appropriate index for these hole cards
	index = GetHandIndex(hand);

	// store the new weight
	WeightedDistribution::SetWeight(index, weight);

}

double HandDistribution::GetWeight(const Hand& hand)
{
	int index;

	// determine the appropriate index for these hole cards
	index = GetHandIndex(hand);

	return WeightedDistribution::GetWeight(index);
}
