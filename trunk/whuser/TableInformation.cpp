#include "Global.h"
#include "TableInformation.h"

TableInformation::TableInformation(void)
{
}

TableInformation::~TableInformation(void)
{
}

int TableInformation::UpdateTableContext(TableContext& context)
{
	table_context.common_pot = context.common_pot;
	table_context.bot_chair = context.bot_chair;
	table_context.dealer_chair = context.dealer_chair;

	table_context.bot_cards[0] = context.bot_cards[0];
	table_context.bot_cards[1] = context.bot_cards[1];

	table_context.betting_round = context.betting_round;

	table_context.bets_to_call = context.bets_to_call;
	table_context.bets_to_raise = context.bets_to_raise;
	table_context.current_bets = context.current_bets;

	return 1;	
}

TableContext* TableInformation::GetTableContext(void)
{
	return &table_context;
}
