#pragma once

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

};


class TableInformation
{
public:
	TableInformation(void);
	~TableInformation(void);
	int UpdateTableContext(TableContext& context);
	TableContext* GetTableContext(void);

private:
	TableContext	table_context;
};
