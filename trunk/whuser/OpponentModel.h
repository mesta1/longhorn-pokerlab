#pragma once

#ifndef _OPPONENTMODEL_H_
#define _OPPONENTMODEL_H_

#include "Global.h"
#include "WeightedHandTable.h"

struct PlayerContext {

	char            name[16];		//player name if known
	double          balance;		//player balance (excluding chips in play)
	unsigned char   cards[2];		//player cards

	double          current_bet;	//player current bet
	double          chips_in_play;	//total in each pot
	int				is_playing;		//1 if still holds cards, 0 if folded or not dealt

};

class OpponentModel
{
public:
	OpponentModel(void);
	~OpponentModel(void);

	int UpdatePlayerContext(PlayerContext& player_context);
	int HasPlayerContextChanged(PlayerContext& player_context);
	PlayerContext* GetPlayerContext(void);

	void DealRandomHand(unsigned char* cards, unsigned char* card_mask) { weight_table.DealRandomHand(cards, card_mask); }

private:

	// p_dealtcards
	// Array of probabilities that the player would hold any given two cards.  The array is
	// in the order (hearts, diamonds, clubs, spades) because that is the order
	// defined by Global.h
	// These are not true probabilities (i.e., odds that he actually holds those cards
	// instead of other cards.
	// e.g., p_cards[0 - 5...] = [AhAh AhAd AhAc AhAs AdAc AdAs ...]
	WeightedHandTable weight_table;

	// p_holecards
	// Graph of probabilities that the play holds any two unique holecards. 
	// This is not useful for calculations, just for graphical display.
	// Array is:
	//
	//		  0  1  2    11 12
	//		  A  K  Q .. 3  2
	//	0	A #
	//	1	K    #	(unsuited)
	//	2	Q       #
	//		:          #
	//	11	3  (suited)  #
	//	12	2				#
	//
	//	Example:	To get P(AKs): p_holecards[0][1]
	//				To get P(AK) : p_holecards[1][0]
	//
	double p_holecards[13][13];

	void UpdateHoleCardGraph(void);			// update p_holecards[][]

	// Player Context
	PlayerContext	context;

	// Player Context History
	// Add to these anything you think is useful.  Note that
	// in future versions we could/should add a connector to
	// to PokerTracker and/or other databases so that we
	// can instantly recall the player's context history.
	// A lot of these variables are stored in PT3.
	double			p_VP$IP;			//percentage player has voluntarily put money into flop
	double			p_preflop_raise;	//percentage player has raised pre-flop
	double			p_cbet;				//percentage player has continuation bet (was last raiser pre-flop then bet on any flop)
	double			p_bluff_river;		//percentage player has bluffed on river (low pwin at showdown and/or bet then folded to raise)
										//this should default zero (unless research shows otherwise) so we don't unreasonably assume any skill.


};

#endif