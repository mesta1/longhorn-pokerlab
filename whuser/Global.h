#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "OpponentModel.h"

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
#define ISCARDBACK(c)	(c==0xff)
#define ISUNKNOWN(c)	(c==0)
#define ISSUITED(c,d)	(SUIT(c)==SUIT(d))
#define UNSUITED		0
#define SUITED			1

#define N_CARDS      52

#define HEARTS   0
#define DIAMONDS 1
#define CLUBS    2
#define SPADES   3

#endif