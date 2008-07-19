#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

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

	struct PlayerContext {
		char            name[16];		//player name if known
		double          balance;		//player balance (excluding chips in play)
		unsigned char   cards[2];		//player cards

		double          current_bet;	//player current bet
		double          chips_in_play;	//total in each pot

		//Player Context History
		//Add to these anything you think is useful.  Note that
		//in future versions we could/should add a connector to
		//to PokerTracker and/or other databases so that we
		//can instantly recall the player's context history.
		//A lot of these variables are stored in PT3.
		double			p_VP$IP;			//percentage player has voluntarily put money into flop
		double			p_preflop_raise;	//percentage player has raised pre-flop
		double			p_cbet;				//percentage player has continuation bet (was last raiser pre-flop then bet on any flop)
		double			p_bluff_river;		//percentage player has bluffed on river (low pwin at showdown and/or bet then folded to raise)
											//this should default zero (unless research shows otherwise) so we don't unreasonably assume any skill.

	};

	struct TableContext {

		unsigned char   common_cards[5];	//common cards
		double			common_pot;			//common pot (excluding current bets)

		double			bot_chair;			//0-9
		double			dealer_chair;		//0-9

		double			bot_deal_position;  //your deal position (1=sblind,2=bblind ... nplayersdealt=dealer). Dealposition will not change as players fold. 
		double			bot_bet_position;	//your bet position (1=sblind,2=bblind,...,nplayersdealt=dealer). Betposition will change as players fold in front of you. 
		double			num_players_dealt;	//number of players dealt (including you) (0-10) 
		double			num_players_playing;//number of players playing (have not folded) (including you) (0-10) 
		double			num_players_behind; //number of players behind you (=num_players_playing - bot_bet_position)

		unsigned char	bot_cards[2];		//bot cards

		double			betting_round;		//betting round (PREFLOP,FLOP,TURN,RIVER)

		double			pot_odds;			//current pot odds (as a percentage) assuming that there is one bet to call
											//TODO: add function to calculate implied pot odds on the fly given table context history
		double			bets_to_call;		//total number of additional bets required to call.
		double			bets_to_raise;		//total number of additional bets required to raise. 
		double			current_bets;		//total number of bets currently in front of you. 

		PlayerContext	player[10];			//player records (limited to 10 for now)
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