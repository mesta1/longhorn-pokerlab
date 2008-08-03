#include "Global.h"
#include "HandAnalyzer.h"
#include "Hand.h"
#include "Debug.h"
#include "Card.h"

#include <ctime>
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
	Debug::log(LTRACE) << "HandAnalyzer::GetPreflopAction(TableInformation& table)" << std::endl;

	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}

ProbabilityTriple HandAnalyzer::GetPostflopAction(TableInformation* table)
{
	Debug::log(LTRACE) << "HandAnalyzer::GetPostflopAction(TableInformation& table)" << std::endl;

	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}

void HandAnalyzer::RunCalculations(TableInformation* table, OpponentModel** players)
{
	Debug::log(LTRACE) << "HandAnalyzer::RunCalculations(TableInformation& table, OpponentModel* players)" << std::endl;

	CalculateProbabilityOfWinning(table, players);

}

int HandAnalyzer::IsHandInList(Hand h, const char* card_list)
{
	char strhand[4]; strhand[3] = NULL;
	char revstrhand[4]; revstrhand[3] = NULL;
	const char ranktochar[]="xA23456789TJQKA";

	// Convert hand from pokerval format to string
	strhand[0] = ranktochar[(h[0]).Rank()];
	strhand[1] = ranktochar[(h[1]).Rank()];
	if ((h[0]).Rank()==(h[1]).Rank())
		strhand[2]=NULL;
	else
		((h[0]).Suit()==(h[1]).Suit()) ? strhand[2]='s' : strhand[2]=' ';
	
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
// NOTE: Speed is absolutely critical in this function.  Will need more
// work to optimize it.
//
//
int				used_card_counter[52];

double	HandAnalyzer::CalculateProbabilityOfWinning(TableInformation* table, OpponentModel** players)
{
	unsigned char	used_card_mask[64];

	Hand			bot_hand;
	Hand			player_hands[10];
	CommonCards		common_cards;

	int				niterations;
	int				win, tie, lose;
	unsigned int	bot_hand_value, opp_hand_value[10];
	unsigned int	best_opp_value;
	int				i,j;

	LARGE_INTEGER	counter_start, counter_end, frequency;
	double			performance_time;

	TableContext*	table_context;

	Debug::log(LTRACE) << "HandAnalyzer::CalculateProbabilityOfWinning()" << std::endl;
	Debug::log(LINFO) << "==== CALCULATING PROBABILITY OF WINNING..." << endl;
	for (i=0; i<52; i++) used_card_counter[i] = 0;

	// Provide initial seed for our random number generator
	// NOTE: deals repeat
	srand((unsigned)time(0));

	niterations = 10000;			// for now we'll default our simulation iteration limit at 1000
	win = tie = lose = 0;		// reset our simulation counters to 0

	// Record our starting time to track performance
	QueryPerformanceCounter(&counter_start);

	// Retrieve our current table context and common cards
	table_context = table->GetCurrentTableContext();

	common_cards.flop1 = table_context->common_cards.flop1;
	common_cards.flop2 = table_context->common_cards.flop2;
	common_cards.flop3 = table_context->common_cards.flop3;
	common_cards.turn = table_context->common_cards.turn;
	common_cards.river = table_context->common_cards.river;

	Debug::log(LINFO)	<< " Common cards: " << common_cards.flop1 << " " 
						<< common_cards.flop2 << " "
						<< common_cards.flop3 << " "
						<< common_cards.turn << " " 
						<< common_cards.river << endl;

	// Retrieve our bot's cards
	bot_hand = table_context->bot_cards;
	Debug::log(LINFO) << "Bot Hand: " << bot_hand << endl;

	///////////////////////////////////////////////////////////
	// MONTE CARLO SIMULATION
	// POKER HAND EVALUATOR
	// Begin the Monte Carlo simulation

	for (i=0; i < niterations; i++)
	{
		// Reset the card mask;
		for (j=0; j<52; j++) used_card_mask[j] = 0;

		// Remove our hole cards from the deck
		used_card_mask[bot_hand[0].Hash()] = 1;
		used_card_mask[bot_hand[1].Hash()] = 1;

		// Remove our common cards from the deck
		if (common_cards.flop1.IsValid()) used_card_mask[common_cards.flop1.Hash()] = 1;
		if (common_cards.flop2.IsValid()) used_card_mask[common_cards.flop2.Hash()] = 1;
		if (common_cards.flop3.IsValid()) used_card_mask[common_cards.flop3.Hash()] = 1;
		if (common_cards.turn.IsValid()) used_card_mask[common_cards.turn.Hash()] = 1;
		if (common_cards.river.IsValid()) used_card_mask[common_cards.river.Hash()] = 1;

		// Deal random cards to each player
		// NOTE: Double check that we deal to all players who were
		// dealt a hand preflop
		for (j=0; j < MAX_PLAYERS; j++)
		{ 
			// Skip our own seat
			if (j == table_context->bot_chair) continue;

			// If this opponent is not playing (i.e., folded or sitting out) do not deal him cards
			if (players[j]->GetPlayerContext()->is_playing == 0)
			{
				player_hands[j].SetCard(0, Card(Card::NOCARD));
				player_hands[j].SetCard(1, Card(Card::NOCARD));
				continue;
			}

			DealCardsToOpponent(players[j], &player_hands[j], used_card_mask); 
		}

		// Roll out random common cards
		if (table_context->betting_round < FLOP) DealFlop(&common_cards, used_card_mask);
		if (table_context->betting_round < TURN) DealTurn(&common_cards, used_card_mask);
		if (table_context->betting_round < RIVER) DealRiver(&common_cards, used_card_mask);
	
		// Determine what our best hand is
//		Debug::log(LDEBUG) << "Evaluating Bot " << j << "..." << endl;
		bot_hand_value = EvaluateBestSevenCardHand(&bot_hand, common_cards);

		// Run through our opponent hands and determine which
		// opponent had the best hand
		best_opp_value = 0;
		for (j=0; j < MAX_PLAYERS; j++)
		{
			// Skip our own seat
			if (j == table_context->bot_chair) continue;

			// Skip players that folded or are sitting out
			if (players[j]->GetPlayerContext()->is_playing == 0) continue;

//			Debug::log(LDEBUG) << "Evaluating Opponent " << j << "..." << endl;
			opp_hand_value[j] = EvaluateBestSevenCardHand(&player_hands[j], common_cards);
			if (opp_hand_value[j] > best_opp_value) best_opp_value = opp_hand_value[j];
		}	

		// Determine whether we win, lose or tie with our
		// opponents best hand
		if (best_opp_value > bot_hand_value)
		{
			lose++;
		}
		else if (best_opp_value == bot_hand_value)
		{
			tie++;
		}
		else
		{
			win++;
		}

	}	

	// Average our win/tie/lose counts to determine our
	// probabilities of each event
	p_win = (double)win / (double)niterations;
	p_tie = (double)tie / (double)niterations;
	p_lose = (double)lose / (double)niterations;

	// Record our ending timer count and calculate our performance_time
	// in seconds for running the simulation.
	QueryPerformanceCounter(&counter_end);
	QueryPerformanceFrequency(&frequency);
	performance_time = (double) (counter_end.LowPart - counter_start.LowPart) / (double) frequency.LowPart;

	Debug::log(LINFO) << "p_win: " << p_win << " p_tie: " << p_tie << " p_lose: " << p_lose << endl;
	Debug::log(LINFO) << "EQUITY: " << (p_win + (p_tie/2)) << endl;
	Debug::log(LINFO) << "nterations: " << niterations << " time: " << performance_time << endl << endl;
/*
	for (i=0; i<13; i++)
	{
		Debug::log(LDEBUG) << "2: " << used_card_counter[(i*4)] << " " << used_card_counter[(i*4)+1] << " " << used_card_counter[(i*4)+2] << " " << used_card_counter[(i*4)+3] << " " << endl;
	}
*/
	return p_win;
}

inline void HandAnalyzer::DealCardsToOpponent(OpponentModel* opponent, Hand* player_hand, unsigned char* card_mask)
{
#if 0
	unsigned char	cards[2];

	// For now, leave out the weighted opponent hand table and assume
	// every card has an equal chance of being held by every opponent

	// If this opponent is not playing (i.e., folded or sitting out) do not deal him cards
	if (opponent->GetPlayerContext()->is_playing == 0)
	{
		player_hand[0] = CARD_NOCARD;
		player_hand[1] = CARD_NOCARD;
		return;
	}

	// Deal a random hand (using weighted distribution from the opponent's hand table).
	// Repeat if the hand we draw includes a used card.
	do {
		opponent->DealRandomHand(cards, card_mask);
	} while (card_mask[ABSOLUTEVAL(cards[0])]==1 || card_mask[ABSOLUTEVAL(cards[1])]==1);

	// We drew two valid cards.  Mark them as used.
	card_mask[ABSOLUTEVAL(cards[0])] = 1;
	card_mask[ABSOLUTEVAL(cards[1])] = 1;
	used_card_counter[ABSOLUTEVAL(cards[0])]++;
	used_card_counter[ABSOLUTEVAL(cards[1])]++;

	player_hand[0] = cards[0];
	player_hand[1] = cards[1];

#else
	int i;

	// For now, leave out the weighted opponent hand table and assume
	// every card has an equal chance of being held by every opponent

	// Pull random cards until we get two that are not marked as already dealt
	do { i = rand() & 63; } while ((i > 51) || (card_mask[i]==1));
	card_mask[i] = 1;
	used_card_counter[i]++;
	player_hand->SetCard(0, Card(i));

 	do { i = rand() & 63; } while ((i > 51) || (card_mask[i]==1));
	card_mask[i] = 1;
	used_card_counter[i]++;
	player_hand->SetCard(1, Card(i));
#endif

	return;
}

inline void HandAnalyzer::DealFlop(CommonCards* common_cards, unsigned char* card_mask)
{
	int				i;

	// Pull random cards until we get three that are not marked as used
	do { i = rand() & 63; } while ((i > 51) || (card_mask[i]==1));
	card_mask[i] = 1;
	used_card_counter[i]++;
	common_cards->flop1 = Card(i);

	do { i = rand() & 63; } while ((i > 51) || (card_mask[i]==1));
	card_mask[i] = 1;
	used_card_counter[i]++;
	common_cards->flop2 = Card(i);

	do { i = rand() & 63; } while ((i > 51) || (card_mask[i]==1));
	card_mask[i] = 1;
	used_card_counter[i]++;
	common_cards->flop3 = Card(i);

	return;
}
inline void HandAnalyzer::DealTurn(CommonCards* common_cards, unsigned char* card_mask)
{
	int				i;

	// Pull random cards until we get one that is not marked as used
	do { i = rand() & 63; } while ((i > 51) || (card_mask[i]==1));
	card_mask[i] = 1;
	used_card_counter[i]++;
	common_cards->turn = Card(i);

	return;
}
inline void HandAnalyzer::DealRiver(CommonCards* common_cards, unsigned char* card_mask)
{
	int				i;

	// Pull random cards until we get one that is not marked as used
	do { i = rand() & 63; } while ((i > 51) || (card_mask[i]==1));
	card_mask[i] = 1;
	used_card_counter[i]++;
	common_cards->river = Card(i);

	return;
}
inline int HandAnalyzer::EvaluateBestSevenCardHand(Hand* player_cards, const CommonCards& common_cards)
{
	return player_cards->Evaluate(common_cards);
}