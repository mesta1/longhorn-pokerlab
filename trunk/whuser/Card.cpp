#include "Card.h"

// Default constructor
Card::Card(void)  
{
	Reset();
}

Card::~Card(void)
{
}

//  Constructor using  a two character string ("Ac" or "Jd")
Card::Card(char* buffer)
{
	if (buffer) FromString(buffer);
	else Reset();
}

// Constructor using a rank value and suit value
Card::Card(int rank, int suit)
{
	Set(rank, suit);
}

// Constructor using an OpenHoldem 8-bit value
Card::Card(unsigned char card)
{
	Set(card);
}

// Clear the card value
void Card::Reset(void)
{
		card = Card::NOCARD;
		hash = Card::ERROR;
}

// Construct the card from a hash value (0-51)
Card::Card(int val)
{
	Set(val);
}

Card& Card::operator=(const Card &rhs) {

    // Only do assignment if RHS is a different object from this.
    if (this != &rhs) {
		card = rhs.card;     
		hash = rhs.hash;
    }

    return *this;
}

Card& Card::operator=(const unsigned char rhs) {

    this->Set(rhs);

    return *this;
}

Card& Card::operator=(const int rhs) {

    this->Set(rhs);

    return *this;
}

int Card::operator==(const Card& rhs) const
{
	if (card == rhs.card) return 1;
	else return 0;
}

int Card::operator!=(const Card& rhs) const
{
	if (card == rhs.card) return 0;
	else return 1;
}

// Set the value of the card from a rank
void Card::Set(int rank, int suit)
{
	card = ((rank<<4) | suit);
	hash = ((rank-2)*4)+(suit-1);
}

// Set the value from an unsigned char (8-bit) card representation
void Card::Set(unsigned char c)
{
	Set(((c>>4)&0x0f), (c&0x0f)); 
}


// Set the card value from a hash value (0-51)
void Card::Set(int h)
{
	if ((h>=0) && (h<52))
	{
		card = _absoluteval_deck[h];
		hash = h;
	}
	else Reset();
}

// Return the card rank (2-14)
int Card::Rank(void) const
{
	return ((card>>4)&0x0f);
}

int Card::Suit(void) const
{
	return (card&0x0f);
}

// Absolute value of card (returns 0-51)
int Card::Hash(void) const
{
	return (hash);
}

// Returns true if the card is unknown 
int Card::IsUnknown() const
{
	return (card==Card::UNKNOWN);
}

// Returns true if the card is a valid value TWO through ACE
int Card::IsValid() const
{
	return ((card!=Card::UNKNOWN) && (card!=Card::NOCARD));
}

// Return a the card text in a three character buffer (including null character)
void Card::ToString(char* buffer) const
{
	if (buffer)
	{
		buffer[0] = _rtoc[Rank()];
		buffer[1] = _stoc[Suit()];
		buffer[2] = NULL;
	}

	return;
}

void Card::FromString(const char* str)
{
	int i, rank, suit;

	if (str)
	{
		for (i=0; i<sizeof(_rtoc); i++) if (_stoc[i]==str[0]) rank=i;
		for (i=0; i<sizeof(_stoc); i++) if (_rtoc[i]==str[1]) suit=i;
	}

	Set(rank, suit);
}

// Send a two character representation of the card into the stream
ostream& operator <<(ostream &os, const Card& c)
{
	char	card_string[3];

	c.ToString(card_string);
	os << card_string;

	return (os);
}

ostream& operator <<(ostream &os, const CommonCards& c)
{
	char	card_string[16];

	c.flop1.ToString(card_string); card_string[2] = ' ';
	c.flop2.ToString(card_string+3); card_string[5] = ' ';
	c.flop3.ToString(card_string+6); card_string[8] = ' ';
	c.turn.ToString(card_string+9); card_string[11] = ' ';
	c.river.ToString(card_string+12); card_string[14] = ' ';
	card_string[15] = 0;
	
	os << card_string;

	return (os);
}