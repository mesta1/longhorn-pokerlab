#pragma once

#include <iostream>
#include <string.h>
using namespace std;

static const char	_rtoc[17] = "[X23456789TJQKA[";
static const char	_stoc[17] = "]cdhsxxxxxxxxxx]";

typedef unsigned char	OH_CARD;

// Listing of cards by absolute value.  Use ABSOLUTEVAL(card)
// to determine corresponding index into this array.
static const unsigned char _absoluteval_deck[52] = 
	{	
		0x21, 0x22, 0x23, 0x24,		// 0	2c 2d 2h 2s
		0x31, 0x32, 0x33, 0x34,		// 4	3c 3d 3h 3s
		0x41, 0x42, 0x43, 0x44,		// 8
		0x51, 0x52, 0x53, 0x54,		// 12
		0x61, 0x62, 0x63, 0x64,		// 16
		0x71, 0x72, 0x73, 0x74,		// 20
		0x81, 0x82, 0x83, 0x84,		// 24
		0x91, 0x92, 0x93, 0x94,		// 28
		0xA1, 0xA2, 0xA3, 0xA4,		// 32
		0xB1, 0xB2, 0xB3, 0xB4,		// 36
		0xC1, 0xC2, 0xC3, 0xC4,		// 40
		0xD1, 0xD2, 0xD3, 0xD4,		// 44
		0xE1, 0xE2, 0xE3, 0xE4		// 48	Ac Ad Ah As
	};

class Card
{
public:
	Card(void);
	Card(int, int);
	Card(int);
	Card(char*);
	Card(unsigned char);
	~Card(void);

	void Set(int, int);
	void Set(int);
	void Set(unsigned char);
	void Reset(void);

	int Rank(void) const;		// return the rank 2-14 (2=2, 14=Ace)
	int Suit(void) const;		// return the suit 1-4 (1=clubs, 4=spades)

	int Hash(void) const;		// returns 0-51 (0 = 2c, 51 = As)

	int IsUnknown(void) const;
	int IsValid(void) const;

	void ToString(char*) const;
	void FromString(const char*);

	Card& operator=(const Card&);
	Card& operator=(const unsigned char);
	Card& operator=(const int);

	int operator==(const Card&) const;
	/*------------------------------------------------------------
	Card Values (from OpenHoldem wiki)

	A single card value is stored in a single 8-bit byte. All 
	bits (7-0) are used. The card rank is stored in the hi-order 
	4 bit nibble (bits 7-4). The card suit is stored in the 
	lo-order 4 bit nibble (bits 3-0). The valid values for card 
	rank are 1-14 as follows:

	Card Rank:
	14 	Ace
	13 	King
	12 	Queen
	11 	Jack
	10-2 	Ten-Two
	1 	Ace (in lo straight)

	Card Suit
	4	Spade
	3 	Heart
	2 	Diamond
	1 	Club
	------------------------------------------------------------*/

	enum CardRank {
		ERROR = -1,
		NOCARD = 0,
		LOWACE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,
		JACK,
		QUEEN,
		KING,
		ACE,
		UNKNOWN
	};

	enum CardSuit {
		CLUBS = 1,
		DIAMONDS,
		HEARTS,
		SPADES
	};

private:
	unsigned char	card;
	int				hash;

};

struct CommonCards {
	Card	flop1;
	Card	flop2;
	Card	flop3;
	Card	turn;
	Card	river;
};
ostream& operator <<(ostream &os, const Card& c);
ostream& operator <<(ostream &os, const CommonCards& c);

#if 0
///////////////////////////////////////////////////////////////
//
//	Macros and definitions for our card values.  Every
//	card is stored as an 8-bit unsigned char as explained
//	below.  The bit values are compatible with OpenHoldem
//	for easy communication with the OpenHoldem API.
//	Might want to turn this into a Card class someday (or not).

/*------------------------------------------------------------
	Card Values (from OpenHoldem wiki)

	A single card value is stored in a single 8-bit byte. All 
	bits (7-0) are used. The card rank is stored in the hi-order 
	4 bit nibble (bits 7-4). The card suit is stored in the 
	lo-order 4 bit nibble (bits 3-0). The valid values for card 
	rank are 1-14 as follows:

	Card Rank:
	14 	Ace
	13 	King
	12 	Queen
	11 	Jack
	10-2 	Ten-Two
	1 	Ace (in lo straight)

	Card Suit
	4	Spade
	3 	Heart
	2 	Diamond
	1 	Club
------------------------------------------------------------*/

// CARD MACROS

#define RANK(c)			((c>>4)&0x0f)					// Rank value of card (2-14) (see above)
#define SUIT(c)			((c>>0)&0x0f)					// Suit value of card (1-4) (see above)
#define ABSOLUTEVAL(c)	(((RANK(c)-2)*4)+(SUIT(c)-1))	// Absolute value of card (returns 0-51)
#define ABVALTOCARD(v)	(_absoluteval_deck[v])
#define SETRANK(c, r)	((c&0xf0) | (r<<4))
#define SETSUIT(c, s)	((c&0x0f) | s)

#define MAKECARD(r,s)	((r<<4) | s)					// Form an unsigned char with card value

#define ISCARDBACK(c)	(c==0xff)
#define ISUNKNOWN(c)	(c==0)
#define ISSUITED(c,d)	(SUIT(c)==SUIT(d))
#define ISVALIDCARD(c)	((c!=0xFF)&&(c!=0xFE)&&(c!=0))

#define UNSUITED		0
#define SUITED			1

#define N_CARDS			52

#define CLUBS			1
#define DIAMONDS		2
#define HEARTS			3
#define SPADES			4

#define	CARD_BACK		0xff
#define	CARD_NOCARD		0x00

static const char	_rtoc[17] = "[X23456789TJQKA[";
static const char	_stoc[17] = "]cdhsxxxxxxxxxx]";

// Convert card value to two character string
inline unsigned char STRTOCARD(const char* str)
{
	int i, rank, suit;

	for (i=0; i<sizeof(_rtoc); i++) if (_stoc[i]==str[0]) rank=i;
	for (i=0; i<sizeof(_stoc); i++) if (_rtoc[i]==str[1]) suit=i;

	return MAKECARD(rank, suit);
}

// Convert two character string to card value
inline void CARDTOSTR(char* buf, unsigned char card)
{
	buf[0] = _rtoc[RANK(card)];
	buf[1] = _stoc[SUIT(card)];

	return;
}

#endif
