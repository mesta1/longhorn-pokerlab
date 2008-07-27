#include "Global.h"
#include "OpponentModel.h"
#include "Debug.h"

#include <conio.h>
#include <stdio.h>
#include <string.h>

OpponentModel::OpponentModel(void)
{
	// Initialize our probability chart.  For now we begin by assuming that
	// before cards are dealt the opponent will play every card
	//for (int i=0; i < 52; i++) p_cards[i] = (double) 1.00;

	// Each time we initialize our probabiliy chart
	// we must update the hole card graph
	UpdateHoleCardGraph();

}

OpponentModel::~OpponentModel(void)
{
}

PlayerContext* OpponentModel::GetPlayerContext(void)
{
	return (&context);
}

void OpponentModel::UpdateHoleCardGraph(void)
{
/*	int i, j;
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
	*/
}

int OpponentModel::UpdatePlayerContext(PlayerContext& player_context)
{
	Debug::log(LTRACE) << "OpponentModel::UpdatePlayerContext(PlayerContext& player_context)" << std::endl;

	context.balance = player_context.balance;

	context.cards[0] = player_context.cards[0];
	context.cards[1] = player_context.cards[1];

	context.current_bet = player_context.current_bet;
	context.is_playing = player_context.is_playing;

	return 1;	
}

int OpponentModel::HasPlayerContextChanged(PlayerContext& player_context)
{
	Debug::log(LTRACE) << "OpponentModel::HasPlayerContextChanged(PlayerContext& player_context)" << std::endl;

	// NOTE: Need to verify that these are the only material changes we are
	// concerned about.

	if (context.balance != player_context.balance) return 1;
	if (context.current_bet != player_context.current_bet) return 1;
	if (context.is_playing != player_context.is_playing) return 1;

	return 0;
}