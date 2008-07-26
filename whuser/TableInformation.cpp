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

	table_context[index].bot_cards[0] = context.bot_cards[0];
	table_context[index].bot_cards[1] = context.bot_cards[1];

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

	// NOTE: Need to verify that these are the only material changes we are
	// concerned about.

	if (table_context[index].betting_round != new_context.betting_round) return 0;	// save context if new betting round
	if (table_context[index].total_pot != new_context.total_pot) return 0;			// save context if anyone bets/raises

	return 1;
}
