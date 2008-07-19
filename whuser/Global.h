#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/*
	OpenHoldem Definitions
	DO NOT ALTER
*/

	// Global entry into OpenHoldem to get symbols
	typedef double (*pfgws_t)( int c, const char* psym, bool& iserr );
	extern	pfgws_t			m_pget_winholdem_symbol;

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

/* ------------------------------------
	
	Global definitions
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
//		double          chips_in_play;	//total in each pot
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

//		double			pot_odds;
		double			bets_to_call;		//total number of additional bets required to call.
		double			bets_to_raise;		//total number of additional bets required to raise. 
		double			current_bets;		//total number of bets currently in front of you. 

		PlayerContext	player[10];			//player records
	};


/*
	char *preflophands[169]
	An array of starting hands ranked by probability of winning
	in a 9-handed game
*/
static char* preflophands[169] =
{
//  0     1     2     3     4     5     6     7     8     9
	"AA ","KK ","QQ ","AKs","AQs","JJ ","KQs","AJs","KJs","ATs", // 0
	"AK ","TT ","QJs","KTs","QTs","JTs","99 ","AQ ","A9s","KQ ", // 1
	"T9s","A8s","K9s","J9s","A5s","Q9s","88 ","AJ ","A7s","A4s", // 2
	"A6s","A3s","KJ ","QJ ","77 ","T8s","K8s","AT ","A2s","98s", // 3
	"K7s","Q8s","J8s","KT ","JT ","66 ","QT ","K6s","87s","K5s", // 4
	"97s","T7s","K4s","76s","55 ","K3s","Q7s","44 ","J7s","33 ", // 5
	"22 ","K2s","86s","65s","Q6s","54s","Q5s","96s","75s","Q4s", // 6
	"T9 ","A9 ","T6s","Q3s","J6s","64s","Q2s","85s","K9 ","J9 ", // 7
	"J5s","53s","Q9 ","A8 ","J4s","J3s","74s","95s","43s","J2s", // 8
	"T5s","A7 ","A5 ","T4s","63s","T8 ","98 ","A4 ","T3s","84s", // 9
	"52s","T2s","A6 ","42s","A3 ","J8 ","K8 ","94s","87 ","73s", // 10
	"Q8 ","93s","32s","A2 ","92s","62s","K7 ","83s","97 ","82s", // 11
	"76 ","K6 ","T7 ","72s","65 ","K5 ","86 ","54 ","J7 ","K4 ", // 12
	"Q7 ","75 ","K3 ","96 ","K2 ","Q6 ","64 ","Q5 ","T6 ","85 ", // 13
	"53 ","Q4 ","J6 ","Q3 ","Q2 ","74 ","43 ","J5 ","95 ","J4 ", // 14
	"63 ","T5 ","J3 ","J2 ","T4 ","52 ","84 ","T3 ","42 ","T2 ", // 15
	"73 ","94 ","32 ","93 ","62 ","92 ","83 ","82 ","72 "		 // 16
};

/*
	double preflophands_prwin[169]
	An array of the probability of winning for the preflophands[]
	starting hands
*/
static double preflophands_prwin[169] = 
{
	31.1,26.1,22.2,20.7,19.3,19.3,18.6,18.1,17.6,17.3,
	17.2,17.2,17.1,16.9,16.6,16.5,15.6,15.5,15.4,15.1,
	14.9,14.8,14.8,14.6,14.5,14.5,14.4,14.3,14.3,14.2,
	14.0,13.9,13.9,13.7,13.7,13.6,13.5,13.4,13.4,13.4,
	13.2,13.2,13.2,13.1,13.1,13.1,12.9,12.8,12.7,12.5,
	12.5,12.4,12.3,12.3,12.3,12.1,12.1,12.1,12.0,12.0,
	12.0,11.9,11.9,11.9,11.7,11.7,11.6,11.4,11.4,11.3,
	11.3,11.2,11.2,11.1,11.1,11.1,10.9,10.9,10.8,10.8,
	10.8,10.8,10.7,10.6,10.6,10.4,10.4,10.3,10.3,10.2,
	10.2,10.1,10.1,10.0,10.0,9.9,9.9,9.8,9.8,9.8,
	9.8,9.7,9.6,9.6,9.5,9.5,9.4,9.4,9.3,9.3,
	9.2,9.2,9.2,9.1,9.1,9.1,9.0,8.9,8.8,8.8,
	8.8,8.6,8.6,8.5,8.5,8.3,8.3,8.2,8.1,8.0,
	8.0,7.9,7.8,7.8,7.6,7.6,7.6,7.4,7.3,7.3,
	7.3,7.1,7.0,6.9,6.8,6.8,6.8,6.7,6.6,6.5,
	6.5,6.4,6.3,6.2,6.2,6.2,6.1,6.0,6.0,5.8,
	5.7,5.6,5.6,5.4,5.4,5.2,5.1,4.9,4.8
};

static char ctonum[14]="23456789TJQKA";  // Not equiv to OH handrank

#define SET_PTRIPLE(a,b,c,d) {a.fold = b; a.checkcall = c; a.betraise = d;}

/////////////////////////////////////
//card macros
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