#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "OpponentModel.h"

#include <string.h>

/*-------------------------------------------------------------------
	OpenHoldem Definitions
	DO NOT ALTER
*/

	// Global entry into OpenHoldem to get symbols
	typedef double (*pfgws_t)( int c, const char* psym, bool& iserr );
	extern	pfgws_t			m_pget_winholdem_symbol;

	//holdem_player
	struct holdem_player {
		char            m_name[16]          ;	//player name if known
		double          m_balance           ;	//player balance 
		double          m_currentbet        ;	//player current bet
		unsigned char   m_cards[2]          ;	//player cards

		unsigned char   m_name_known    : 1 ;	//0=no 1=yes
		unsigned char   m_balance_known : 1 ;	//0=no 1=yes
		unsigned char   m_fillerbits    : 6 ;	//filler bits
		unsigned char   m_fillerbyte        ;	//filler bytes
	};

	//holdem_state (passed from OpenHoldem with each DLL call
	//contains more information than the symbols which are
	//calculated with each status update
	struct holdem_state {
		char            m_title[64]         ;	//table title
		double          m_pot[10]           ;	//total in each pot

		unsigned char   m_cards[5]          ;	//common cards

		unsigned char   m_is_playing    : 1 ;	//0=sitting-out, 1=sitting-in
		unsigned char   m_is_posting    : 1 ;	//0=autopost-off, 1=autopost-on
		unsigned char   m_fillerbits    : 6 ;	//filler bits

		unsigned char   m_fillerbyte        ;	//filler byte
		unsigned char   m_dealer_chair      ;	//0-9

		holdem_player   m_player[10]        ;	//player records
	};

/*-------------------------------------------------------------------
	
	Global definitions

	These are unique to the bot engine and may be architected
	at will.
*/

struct ProbabilityTriple {
	double			fold;		//probability of folding
	double			checkcall;	//probability of check / call
	double			betraise;	//probability of bet / raise
};

enum ACTION {
	FOLD = 0,
	CHECKCALL,
	BETRAISE
};
enum BETTING_ROUND {
	PREFLOP = 0,
	FLOP,
	TURN,
	RIVER
};

// Use this definition for the maximum number of opponents.  We
// may want to make this configurable.  Note that this is not
// the same as the maximum players at a table (which may be 9)
#define	MAX_OPPONENTS		9

#define SET_PTRIPLE(a,b,c,d) {a.fold = b; a.checkcall = c; a.betraise = d;}

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

// Listing of cards by absolute value.  Use ABSOLUTEVAL(card)
// to determine corresponding index into this array.
static const unsigned char _absoluteval_deck[52] = 
	{	
		0x21, 0x22, 0x23, 0x24,		// 2c 2d 2h 2s
		0x31, 0x32, 0x33, 0x34,		// 3c 3d 3h 3s
		0x41, 0x42, 0x43, 0x44,
		0x51, 0x52, 0x53, 0x54,
		0x61, 0x62, 0x63, 0x64,
		0x71, 0x72, 0x73, 0x74,
		0x81, 0x82, 0x83, 0x84,
		0x91, 0x92, 0x93, 0x94,
		0xA1, 0xA2, 0xA3, 0xA4,
		0xB1, 0xB2, 0xB3, 0xB4,
		0xC1, 0xC2, 0xC3, 0xC4,
		0xD1, 0xD2, 0xD3, 0xD4,
		0xE1, 0xE2, 0xE3, 0xE4		// Ac Ad Ah As
	};

#endif