#include "Global.h"
#include "HandAnalyzer.h"
#include "Hand.h"
#include "Debug.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

HandAnalyzer::HandAnalyzer(void)
{
}

HandAnalyzer::~HandAnalyzer(void)
{
}

ProbabilityTriple HandAnalyzer::GetPreflopAction(TableInformation* table)
{
	Debug::log(LTRACE) << "HandAnalyzer::GetPreflopAction(TableInformation* table)" << std::endl;

	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}

ProbabilityTriple HandAnalyzer::GetPostflopAction(TableInformation* table)
{
	Debug::log(LTRACE) << "HandAnalyzer::GetPostflopAction(TableInformation* table)" << std::endl;

	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}

int HandAnalyzer::isHandInList(unsigned char c[2], const char* card_list)
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

// NOTE:  When developing this function make an effort to keep it
// thread-safe.  For more info read:
// http://publib.boulder.ibm.com/infocenter/systems/index.jsp?topic=/com.ibm.aix.genprogc/doc/genprogc/writing_reentrant_thread_safe_code.htm
//
// NOTE: Player limits are hardcoded to 10
//
double	HandAnalyzer::CalculateProbabilityOfWinning(TableInformation* table, OpponentModel* players)
{
	unsigned char	used_card_mask[52];

	unsigned char	player_cards[10][2];
	unsigned char	common_cards[5];

	double			p_win, p_tie, p_lose;
	int				win, tie, lose;
	int				bot_hand_value, opp_hand_value[10];
	int				i,j;

	LARGE_INTEGER	counter_start, counter_end, frequency;
	double			performance_time;

	TableContext*	table_context;

	Debug::log(LDEBUG4) << "HandAnalyzer::CalculateProbabilityOfWinning(TableInformation* table, OpponentModel* players)" << std::endl;

	int	niterations = 1000;		// for now we'll default our simulation iteration limit at 1000
	win = tie = lose = 0;		// reset our simulation counters to 0

	// Record our starting time to track performance
	QueryPerformanceCounter(&counter_start);

	// Retrieve our current table context
	table_context = table->GetCurrentTableContext();
	for (i=0; i < 5; i++) common_cards[i] = table_context->common_cards[i];

	// MONTE CARLO SIMULATION
	// POKER EVALUATOR
	// Begin the Monte Carlo simulation
	for (i=0; i < niterations; i++)
	{
		// Deal random cards to each player
		for (j=0; j < 10; j++) { DealCardsToOpponent(&players[j], player_cards[j], used_card_mask); }

		// Roll out random common cards
		if (table_context->betting_round < FLOP) DealFlop(common_cards, used_card_mask);
		if (table_context->betting_round < TURN) DealTurn(common_cards, used_card_mask);
		if (table_context->betting_round < RIVER) DealRiver(common_cards, used_card_mask);
	
		// Determine what our best hand is
		bot_hand_value = EvaluateBestSevenCardHand(table_context->bot_cards, common_cards);

		for (j=0; j < 10; j++) {

			opp_hand_value[j] = EvaluateBestSevenCardHand(player_cards[j], common_cards);
			if (opp_hand_value[j] > bot_hand_value)
			{
				lose++;
			}
			else if (opp_hand_value[j] == bot_hand_value)
			{
				tie++;
			}
			else
			{
				win++;
			}
		}	

		p_win = win / niterations;
		p_tie = tie / niterations;
		p_lose = lose / niterations;
	}	

	// Record our ending timer count and calculate our performance_time
	// in seconds for running the simulation.
	QueryPerformanceCounter(&counter_end);
	QueryPerformanceFrequency(&frequency);
	performance_time = (double) (counter_end.LowPart - counter_start.LowPart) / (double) frequency.LowPart;

	return p_win;
}

inline void HandAnalyzer::DealCardsToOpponent(OpponentModel* opponent, unsigned char* player_cards, unsigned char* card_mask)
{
}

inline void HandAnalyzer::DealFlop(unsigned char* common_cards, unsigned char* card_mask)
{
}
inline void HandAnalyzer::DealTurn(unsigned char* common_cards, unsigned char* card_mask)
{
}
inline void HandAnalyzer::DealRiver(unsigned char* common_cards, unsigned char* card_mask)
{
}
inline int HandAnalyzer::EvaluateBestSevenCardHand(unsigned char* player_cards, unsigned char* common_cards)
{
	Hand	hand;
	int		peval_value;

	hand.AddCard(player_cards[0]);
	hand.AddCard(player_cards[1]);

	hand.AddCard(common_cards[0]);
	hand.AddCard(common_cards[1]);
	hand.AddCard(common_cards[2]);
	hand.AddCard(common_cards[3]);
	hand.AddCard(common_cards[4]);

	peval_value = hand.Evaluate();

	return peval_value;

}