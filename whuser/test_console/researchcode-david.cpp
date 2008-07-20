#include "stdafx.h"

#include <conio.h>
#include <stdio.h>
#include <string.h>

/*
	char *preflophands[169]
	An array of starting hands ranked by probability of winning
	in a 9-handed game
*/
char *preflophands[169] =
{
	"AA ","KK ","QQ ","AKs","AQs","JJ ","KQs","AJs","KJs","ATs",
	"AK ","TT ","QJs","KTs","QTs","JTs","99 ","AQ ","A9s","KQ ",
	"T9s","A8s","K9s","J9s","A5s","Q9s","88 ","AJ ","A7s","A4s",
	"A6s","A3s","KJ ","QJ ","77 ","T8s","K8s","AT ","A2s","98s",
	"K7s","Q8s","J8s","KT ","JT ","66 ","QT ","K6s","87s","K5s",
	"97s","T7s","K4s","76s","55 ","K3s","Q7s","44 ","J7s","33 ",
	"22 ","K2s","86s","65s","Q6s","54s","Q5s","96s","75s","Q4s",
	"T9 ","A9 ","T6s","Q3s","J6s","64s","Q2s","85s","K9 ","J9 ",
	"J5s","53s","Q9 ","A8 ","J4s","J3s","74s","95s","43s","J2s",
	"T5s","A7 ","A5 ","T4s","63s","T8 ","98 ","A4 ","T3s","84s",
	"52s","T2s","A6 ","42s","A3 ","J8 ","K8 ","94s","87 ","73s",
	"Q8 ","93s","32s","A2 ","92s","62s","K7 ","83s","97 ","82s",
	"76 ","K6 ","T7 ","72s","65 ","K5 ","86 ","54 ","J7 ","K4 ",
	"Q7 ","75 ","K3 ","96 ","K2 ","Q6 ","64 ","Q5 ","T6 ","85 ",
	"53 ","Q4 ","J6 ","Q3 ","Q2 ","74 ","43 ","J5 ","95 ","J4 ",
	"63 ","T5 ","J3 ","J2 ","T4 ","52 ","84 ","T3 ","42 ","T2 ",
	"73 ","94 ","32 ","93 ","62 ","92 ","83 ","82 ","72 "
};

/*
	double preflophands_prwin[169]
	An array of the probability of winning for the preflophands[]
	starting hands
*/
double preflophands_prwin[169] = 
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

/*
	int preflophands_threshold[5]
	This variable is an array of the minimum preflophands_prwin[] value fo
	each action below
*/
#define		preflop_FOLD		0;
#define		preflop_MAKE1		1;	//	call one bet
#define		preflop_MAKE2		2;	//	call two bets / raise if unraised
#define		preflop_MAKE3		3;	//	call three bets / raise-if-reraised
#define		preflop_CAP			4;	//	cap
int preflophands_threshold[5];

static char ctonum[14]="23456789TJQKA";

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

#define Rank_2      0
#define Rank_3      1
#define Rank_4      2
#define Rank_5      3
#define Rank_6      4
#define Rank_7      5
#define Rank_8      6
#define Rank_9      7
#define Rank_TEN    8
#define Rank_JACK   9
#define Rank_QUEEN  10
#define Rank_KING   11
#define Rank_ACE    12
#define Rank_COUNT  13
#define Rank_FIRST  Rank_2
#define Rank_LAST   Rank_ACE

#define Suit_HEARTS   0
#define Suit_DIAMONDS 1
#define Suit_CLUBS    2
#define Suit_SPADES   3

	struct ProbabilityTriple {
		double			fold;		//probability of folding
		double			checkcall;	//probability of check / call
		double			betraise;	//probability of bet / raise
	};

/*
	ishandinlist(unsigned char c[2], const char* card_list)

	See if a given hand is in a list of hands
	card_list must be formatted as follows:
	"AA AKs 77 72 72s QQ KQs"
*/
int ishandinlist(unsigned char c[2], const char* card_list)
{
	char strhand[4]; strhand[3] = NULL;
	char revstrhand[4]; revstrhand[3] = NULL;
	const char ranktochar[]="xA23456789TJQKA";

	// Convert hand from pokerval format to string
	strhand[0] = ranktochar[RANK(c[0])];
	strhand[1] = ranktochar[RANK(c[1])];
	if (RANK(c[0])==RANK(c[1]))
		strhand[2]=NULL;
	else
		ISSUITED(c[0],c[1]) ? strhand[2]='s' : strhand[2]=' ';
	
	// Make mirror image of string in case cards are in reverse order
	revstrhand[1] = strhand[0];
	revstrhand[0] = strhand[1];
	revstrhand[2] = strhand[2];

	if (strstr(card_list, strhand)) return 1; // found card in string list
	if (strstr(card_list, revstrhand)) return 1; // found card in string list
		
	return 0;  // found no match
}

void testincomerate()
{
/*	int			i, j;
	int			k, vndx; //Matrix 2008-05-08
    char		*ptr;
	double		incomerate[169];

	for (i=0; i<169; i++)
	{
        ptr=prwhandrank169[i];
        j=strchr(ctonum,*ptr)-ctonum;
		k=strchr(ctonum,*(ptr+1))-ctonum;

		if (*(ptr+2)=='s')
		{
			incomerate[i] = startinghands_suited[j][k];
		}
		else
		{
			incomerate[i] = startinghands_unsuited[j][k];
		}
		
		printf("%5d ", (int)incomerate[i]);
		if (((i+1)%10)==0) printf("\n");

	}

	srand((unsigned)time(0)); 
    double random_integer = rand()/(RAND_MAX + 1.0); 

	// Normalize the preflop strength matrix
	int i = 0;
	double maxval = 0;
	for (i=0; i < 169; i++) { 
		if (preflophands_prwin[i] > maxval)
			maxval = preflophands_prwin[i];
	}
	for (i=0; i < 169; i++) { 
		preflophands_prwin[i] /= maxval;

*/
}

int testhandlist()
{
	unsigned char cards[2];
	ProbabilityTriple	ptriple;

	cards[0] = 0x63;
	cards[1] = 0x73;

	// AA or KK
	if (ishandinlist(cards, "AA KK")) {
		SET_PTRIPLE(ptriple, 0.0,0.1,0.9);
		return 1;
	}

	// AKs or QQ
	if (ishandinlist(cards, "AKs QQ TT JJ 97 67s 98s 22")) {
		SET_PTRIPLE(ptriple, 0.0,0.1,0.9);
		return 1;
	}

	ptriple.fold = 0.7;
	ptriple.checkcall = 0.3;
	ptriple.betraise = 0.0;

	return 0;
}

#define HEARTS   0
#define DIAMONDS 1
#define CLUBS    2
#define SPADES   3
double p_cards[52];
double p_holecards[13][13];

void UpdateHoleCardGraph(void)
{
	int i, j;
	int idx1, idx2;
	double p;

	// For every possible rank of card 1...
	for (i=0; i<13; i++)		
	{
		idx1 = i*4;

		/////////////////////////////////////////////////////
		// Calculate pocket pair probabilities
		p = (p_cards[idx1+HEARTS] * p_cards[idx1+DIAMONDS]) +
			(p_cards[idx1+HEARTS] * p_cards[idx1+CLUBS]) +
			(p_cards[idx1+HEARTS] * p_cards[idx1+SPADES]) +
			(p_cards[idx1+DIAMONDS] * p_cards[idx1+CLUBS]) +
			(p_cards[idx1+DIAMONDS] * p_cards[idx1+SPADES]) +
			(p_cards[idx1+CLUBS] * p_cards[idx1+SPADES]);

		p /= 6;	// normalize to 0.00-1.00
		p_holecards[i][i] = p;

		// For every possible rank of card 2...
		for (j=0; j<13; j++)	
		{
			if (i==j) continue;	// skip pocket pairs because they're already done.

			idx2 = j*4;

			/////////////////////////////////////////////////////
			// Calculate suited hand probabilities
			p = (p_cards[idx1+HEARTS] * p_cards[idx2+HEARTS]) +
				(p_cards[idx1+DIAMONDS] * p_cards[idx2+DIAMONDS]) +
				(p_cards[idx1+CLUBS] * p_cards[idx2+CLUBS]) +
				(p_cards[idx1+SPADES] * p_cards[idx2+SPADES]);

			p /= 4; // normalize to 0.00-1.00
			p_holecards[i][j] = p;

			/////////////////////////////////////////////////////
			// Calculate unsuited hand probabilities
			p = (p_cards[idx1+HEARTS] * p_cards[idx2+DIAMONDS]) +
				(p_cards[idx1+HEARTS] * p_cards[idx2+CLUBS]) +
				(p_cards[idx1+HEARTS] * p_cards[idx2+SPADES]) +
				(p_cards[idx1+DIAMONDS] * p_cards[idx2+CLUBS]) +
				(p_cards[idx1+DIAMONDS] * p_cards[idx2+SPADES]) +
				(p_cards[idx1+CLUBS] * p_cards[idx2+SPADES]);

			p /= 6;	// normalize to 0.00-1.00
			p_holecards[j][i] = p;
		}
	}
}
void OpponentModel(void)
{
	// Initialize our probability chart.  For now we begin by assuming that
	// before cards are dealt the opponent will play every card
	for (int i=0; i < 52; i++) p_cards[i] = (double) 1.00;

	// Each time we initialize our probabiliy chart
	// we must update the hole card graph
	UpdateHoleCardGraph();

	for (int j=0; j < 13; j++)
	{
		for (int k=0; k < 13; k++) _cprintf("%04f ", p_holecards[j][k]);
		_cprintf("\n");
	}
}

