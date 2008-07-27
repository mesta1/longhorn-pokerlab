#include "Global.h"
#include "Hand.h"
#include "Debug.h"
#include "inlines/eval.h"

Hand::Hand(void)
{
	for (int i=0; i<7; i++) cards[i] = CARD_NOCARD;
	m_size = 0;
	hand_value = 0;
}

Hand::~Hand(void)
{
}
/*
unsigned int &Hand::operator[] (int index)
{
	if (index < 7) return cards[index];
	else return cards[0];  // kludge
}
*/

int Hand::AddCard(unsigned char card)
{
	if (m_size > 6) return -1;

	// We must convert our hand from our format (8-bit):
	// | 7  6  5  4   3  2  1  0 |
	// |    rank    |    suit    |
	//
	// to the poker-eval format (16-bit)
	int rank = RANK(card)-2;
	int suit = SUIT(card) == CLUBS ? StdDeck_Suit_CLUBS :
			   SUIT(card) == DIAMONDS ? StdDeck_Suit_DIAMONDS :
			   SUIT(card) == HEARTS ? StdDeck_Suit_HEARTS :
			   SUIT(card) == SPADES ? StdDeck_Suit_SPADES : 0;

	cards[m_size] = StdDeck_MAKE_CARD(rank, suit);
	m_size++;

	return 0;
}

int Hand::Evaluate(void)
{
    CardMask            eval_cards;

	// Set a poker-eval card mask with the values of all the cards
	CardMask_RESET(eval_cards);
	for(int i=0 ; i<m_size ; ++i)
	{
		CardMask_SET(eval_cards, cards[i]);
	}

	// Get my handval/pokerval
	hand_value = Hand_EVAL_N(eval_cards, m_size);

	return hand_value;
}

void Hand::Reset(void)
{
	m_size = 0;
	hand_value = 0;
}

void Hand::ToString(char* str)
{
	HandVal_toString(hand_value, str);

	return;
}
