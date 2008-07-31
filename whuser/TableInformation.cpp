#include "Global.h"
#include "TableInformation.h"
#include "Debug.h"

TableInformation::TableInformation(void)
{
	index = 0;
}

TableInformation::~TableInformation(void)
{
}

// TableContext accessor function for the ostream << operator
// usage: cout << (TableContext) mycontext << endl;
//
ostream& operator <<(ostream &os, const TableContext& context)
{
//	char buffer[256];

	os << "TABLE_CONTEXT:" << endl;

//	CARDTOSTR(&buffer[0], context.bot_cards[0]);
//	CARDTOSTR(&buffer[2], context.bot_cards[1]);
//	buffer[4] = 0;
//	os << "bot_cards: " << buffer;

	os << "bot_cards: " << context.bot_cards;
/*
	CARDTOSTR(&buffer[0], context.common_cards[0]);
	CARDTOSTR(&buffer[2], context.common_cards[1]);
	CARDTOSTR(&buffer[4], context.common_cards[2]);
	CARDTOSTR(&buffer[6], context.common_cards[3]);
	CARDTOSTR(&buffer[8], context.common_cards[4]);
	buffer[10] = 0;
	os << " common_cards: " << buffer << " betting_round: " << context.betting_round << endl;
*/
	os << " common_cards: " << context.common_cards.flop1 << " " << 
							   context.common_cards.flop2 << " " << 
							   context.common_cards.flop3 << " " << 
							   context.common_cards.turn << " " << 
							   context.common_cards.river;
	os << " betting_round: " << context.betting_round << endl;

	os << "common_pot: " << context.common_pot << " context.total_pot: " << context.total_pot << endl;
	os << "bot_deal_position: " << context.bot_deal_position << " bot_bet_position: " << context.bot_bet_position << endl;
	os << "num_players_dealt: " << context.num_players_dealt << " num_players_playing: " << context.num_players_playing << endl;
	os << "num_players_behind: " << context.num_players_behind << endl;

	return os;
}

// Append the newest table context information to our stored history
int TableInformation::UpdateTableContext(TableContext& context)
{
	Debug::log(LTRACE) << "TableInformation::UpdateTableContext(TableContext& context)" << std::endl;

	index++;

	// Make sure we haven't accumulated more than 1024 context updates
	// during the current hand.  This _should_ be a good limit for now because
	// Limit Hold 'Em has max 10 players with max 4 betting levels and max 5 
	// betting rounds = 200 occurences.
	if (index == 1024) 
	{
		Debug::log(LERROR) << "ERROR: Reached upperlimit of class TableInformation" << std::endl;
		return 0;
	}

	table_context[index].common_pot = context.common_pot;
	table_context[index].total_pot = context.total_pot;

	table_context[index].bot_chair = context.bot_chair;
	table_context[index].dealer_chair = context.dealer_chair;

	table_context[index].bot_cards = context.bot_cards;

	table_context[index].common_cards.flop1 = context.common_cards.flop1;
	table_context[index].common_cards.flop2 = context.common_cards.flop2;
	table_context[index].common_cards.flop3 = context.common_cards.flop3;
	table_context[index].common_cards.turn = context.common_cards.turn;
	table_context[index].common_cards.river = context.common_cards.river;

	table_context[index].betting_round = context.betting_round;

	table_context[index].bets_to_call = context.bets_to_call;
	table_context[index].bets_to_raise = context.bets_to_raise;
	table_context[index].current_bets = context.current_bets;

	return 1;	
}

TableContext* TableInformation::GetCurrentTableContext(void)
{
	return &table_context[index];
}

// Returns true if the saved table context differs from the given context in a material way
int TableInformation::HasTableContextChanged(TableContext& new_context)
{
	Debug::log(LTRACE) << "TableInformation::HasTableContextChanged(TableContext& new_context)" << std::endl;
//	Debug::log(LDEBUG) << "table_context[index].betting_round: " << table_context[index].betting_round << std::endl;
//	Debug::log(LDEBUG) << "table_context[index].total_pot: " << table_context[index].total_pot << std::endl;
//	Debug::log(LDEBUG) << "new_context.betting_round: " << new_context.betting_round << std::endl;
//	Debug::log(LDEBUG) << "new_context.total_pot: " << new_context.total_pot << std::endl;

	// If this is the first call for the hand return true;
	if (index == 0) return 1;

	// NOTE: Need to verify that these are the only material changes we are
	// concerned about.

	if (table_context[index].betting_round != new_context.betting_round) return 1;	// save context if new betting round
	if (table_context[index].total_pot != new_context.total_pot) return 1;			// save context if anyone bets/raises

	return 0;
}
