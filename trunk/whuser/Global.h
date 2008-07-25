#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "OpponentModel.h"

#include <string.h>

void DEBUG_PRINT(char* fmt, ...);

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

// Use this definition for the maximum number of players.  We
// may want to make this configurable.  Note that this is not
// the same as the maximum players at a table (which may be 9)
#define	MAX_PLAYERS		10;

#define SET_PTRIPLE(a,b,c,d) {a.fold = b; a.checkcall = c; a.betraise = d;}

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

#define RANK(c)			((c>>4)&0x0f)
#define SUIT(c)			((c>>0)&0x0f)

#define SETRANK(c, r)	((c&0xf0) | (r<<4))
#define SETSUIT(c, s)	((c&0x0f) | s)
#define MAKECARD(r,s)	((r<<4) | s)
#define ISCARDBACK(c)	(c==0xff)
#define ISUNKNOWN(c)	(c==0)
#define ISSUITED(c,d)	(SUIT(c)==SUIT(d))
#define UNSUITED		0
#define SUITED			1

#define N_CARDS      52

#define CLUBS		1
#define DIAMONDS	2
#define HEARTS		3
#define SPADES		4

#define		CARD_BACK		0xff
#define		CARD_NOCARD		0x00

const char	_stoc[] = "xx23456789TJQKA";
const char	_rtoc[] = "xcdhs";

inline unsigned char STRINGTOCARD(const char* str)
{
	int i, rank, suit;

	for (i=0; i<sizeof(_stoc); i++) if (_stoc[i]==str[0]) rank=i;
	for (i=0; i<sizeof(_rtoc); i++) if (_rtoc[i]==str[1]) suit=i;

	return MAKECARD(rank, suit);
}


#endif