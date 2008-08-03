#pragma once

#include "Card.h"
#include "Hand.h"

#include <iostream>
#include <string.h>
using namespace std;

struct TableContext {

	char            table_name[64];		//poker site table name
	double			hand_number;		//poker site hand number

	CommonCards		common_cards;		//common cards
	double			common_pot;			//common pot (excluding current bets)
	double			total_pot;			//total pot (including common pot and current bets)

	int				bot_chair;			//0-9
	int				dealer_chair;		//0-9

	int				bot_deal_position;  //your deal position (1=sblind,2=bblind ... nplayersdealt=dealer). Dealposition will not change as players fold. 
	int				bot_bet_position;	//your bet position (1=sblind,2=bblind,...,nplayersdealt=dealer). Betposition will change as players fold in front of you. 
	int				num_players_dealt;	//number of players dealt (including you) (0-10) 
	int				num_players_playing;//number of players playing (have not folded) (including you) (0-10) 
	int				num_players_behind; //number of players behind you (=num_players_playing - bot_bet_position)

	Hand			bot_cards;			//bot cards

	int				betting_round;		//betting round (PREFLOP,FLOP,TURN,RIVER)

	double			bets_to_call;		//total number of additional bets required to call.
	double			bets_to_raise;		//total number of additional bets required to raise. 
	double			current_bets;		//total number of bets currently in front of you. 

};

struct TableStatistics {

	double			p_win;				//probability that the bot hand will win at showdown
	double			p_tie;				//probability that the bot hand will tie at showdown
	double			p_lose;				//probability that the bot hand will lose at showdown
	double			equity_showdown;	//showdown equity = p_win + (p_tie/2)
	double			pot_odds;			//current pot odds (as a percentage) assuming that there is one bet to call
										//TODO: add function to calculate implied pot odds on the fly given table context history
};

ostream& operator <<(ostream &os, const TableContext& context);

class TableInformation
{
public:
	TableInformation(void);
	~TableInformation(void);
	int UpdateTableContext(const TableContext& context);
	TableContext* GetCurrentTableContext(void);
	int HasTableContextChanged(const TableContext& context);
	int IsBotHandConfirmed(void) const;
	void SaveCurrentTableContext(const TableContext& new_context);

private:
	TableContext	table_context[1024];	// array of TableContext structures to keep history during the hand
											// we can create functions to examine the history of the hand
	TableStatistics table_statistics;		// global storage for the most recently calculated statistics

	int				bot_hand_confirmed_;	// reassurance that we are free to calculate and evaluate
	int				index_;
};
