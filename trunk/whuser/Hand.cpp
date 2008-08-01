#include "Global.h"
#include "Hand.h"
#include "Debug.h"
#include "inlines/eval.h"

// Send a 4 character representation of our hand into the stream (i.e., "Ac Ah" or "Jc Ts")
// TODO: Change this to send common cards too (3, 4, or 5) 
ostream& operator <<(ostream &os, const Hand& h)
{
	os << (Card)(h[0]);
	os << " ";
	os << (Card)(h[1]);

	return (os);
}

// Use the subscript operator to retrieve either of our hole cards (const)
const Card Hand::operator[](const int index) const
{
	if ((index >= 0) && (index < 2))
	{
		// Return a Card class with of the desired card
		return (cards[index]);
	}
	else return Card(Card::NOCARD);  // kludge (returns blank card)
}

// Constructor for a generic, blank Hand class
Hand::Hand(void)
{
	Reset();
}

// Constructor for a Hand class initialized with two Cards
Hand::Hand(const Card& card1, const Card& card2)
{
	pokereval_cards[0] = ConvertCardToPokerEvalCard(card1);
	pokereval_cards[1] = ConvertCardToPokerEvalCard(card2);
	cards[0] = card1;
	cards[1] = card2;
	hand_value = 0;
}

// Destructor
Hand::~Hand(void)
{
}

void Hand::SetCard(const int index, const Card& card)
{
	if ((index >= 0) && (index < 2))
	{
		if (card.IsValid())
		{
			pokereval_cards[index] = ConvertCardToPokerEvalCard(card);
			cards[index] = card;
			hand_value = 0;
		}
	}
	else
	{
		Debug::log(LERROR) << "Attempted to set card out of bounds in Hand::SetCard()! index=" << index << endl;
	}
}

// Evaluate the best 7 card hand given our hole cards and 5 common_cards
// TODO: Change this to evalute best 5+ card hand
// TODO: Change this to store common cards (so we can print it out later)
unsigned int Hand::Evaluate(const CommonCards& common_cards)
{
    CardMask            eval_cards;
//	char				buf[256];

//	Debug::log(LDEBUG) << "Hand: " << (*this) << std::endl;
//	Debug::log(LDEBUG) << "Common cards: " << common_cards << std::endl;

	// Set a poker-eval card mask with the values of all the cards
	CardMask_RESET(eval_cards);
	CardMask_SET(eval_cards, pokereval_cards[0]); 
	CardMask_SET(eval_cards, pokereval_cards[1]); 
	CardMask_SET(eval_cards, ConvertCardToPokerEvalCard(common_cards.flop1));
	CardMask_SET(eval_cards, ConvertCardToPokerEvalCard(common_cards.flop2));
	CardMask_SET(eval_cards, ConvertCardToPokerEvalCard(common_cards.flop3));
	CardMask_SET(eval_cards, ConvertCardToPokerEvalCard(common_cards.turn));
	CardMask_SET(eval_cards, ConvertCardToPokerEvalCard(common_cards.river));

//	StdDeck_maskToString(eval_cards, buf);
//	Debug::log(LDEBUG) << "Mask: " << buf << std::endl;

	// Get my handval/pokerval
	hand_value = Hand_EVAL_N(eval_cards, 7);

//	StdRules_HandVal_toString(hand_value, buf);
//	Debug::log(LDEBUG) << "Hand_EVAL_N: " << buf << std::endl << std::endl;

	return hand_value;
}

// Reset the class to blank;
void Hand::Reset(void)
{
	pokereval_cards[0] = 0;
	pokereval_cards[1] = 0;
	cards[0] = Card::NOCARD;
	cards[1] = Card::NOCARD;
	hand_value = 0;
}

void Hand::HandValueToString(char* str)
{
	HandVal_toString(hand_value, str);

	return;
}

int	Hand::ConvertCardToPokerEvalCard(const Card& card) const
{
	int rank, suit;

	// We must convert our hand from our format (8-bit):
	// | 7  6  5  4   3  2  1  0 |
	// |    rank    |    suit    |
	//
	// to the poker-eval format (16-bit)
	//        (rank*4)+suit
	//
	rank = card.Rank()-2;
	suit = card.Suit() == Card::CLUBS ? StdDeck_Suit_CLUBS :
			   card.Suit() == Card::DIAMONDS ? StdDeck_Suit_DIAMONDS :
			   card.Suit() == Card::HEARTS ? StdDeck_Suit_HEARTS :
			   card.Suit() == Card::SPADES ? StdDeck_Suit_SPADES : 0;

	return (StdDeck_MAKE_CARD(rank, suit));
}
/*
int Hand::ConvertCardToPokerEvalCard(const unsigned char card) const
{
	int rank, suit;

	// We must convert our hand from our format (8-bit):
	// | 7  6  5  4   3  2  1  0 |
	// |    rank    |    suit    |
	//
	// to the poker-eval format (16-bit)
	//        (rank*4)+suit
	//
	rank =	RANK(card)-2;
	suit =	SUIT(card) == Card::CLUBS ? StdDeck_Suit_CLUBS :
			SUIT(card) == Card::DIAMONDS ? StdDeck_Suit_DIAMONDS :
			SUIT(card) == Card::HEARTS ? StdDeck_Suit_HEARTS :
			SUIT(card) == Card::SPADES ? StdDeck_Suit_SPADES : 0;

	return (StdDeck_MAKE_CARD(rank, suit));
}
*/
// Convert our member variable card (poker-eval format) to our own Card class
Card Hand::ConvertPokerEvalCardToCard(int card) const
{
	int rank, suit;

	// We must convert the poker-eval format (16-bit)
	//        (rank*4)+suit
	//
	// to our hand from our format (8-bit):
	// | 7  6  5  4   3  2  1  0 |
	// |    rank    |    suit    |
	//
	rank =	StdDeck_RANK(card)+2;
	suit =	StdDeck_SUIT(card) == StdDeck_Suit_CLUBS ? Card::CLUBS :
			StdDeck_SUIT(card) == StdDeck_Suit_DIAMONDS ? Card::DIAMONDS :
			StdDeck_SUIT(card) == StdDeck_Suit_HEARTS ? Card::HEARTS :
			StdDeck_SUIT(card) == StdDeck_Suit_SPADES ? Card::SPADES : 0;

	return (Card(rank, suit));
}

int Hand::IsValid() const
{
	return (cards[0].IsValid() && cards[1].IsValid());
}

