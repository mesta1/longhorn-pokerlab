#include "Global.h"
#include "TableInformation.h"
#include "Debug.h"

TableInformation::TableInformation(void)
{
	index_ = -1;
	bot_hand_confirmed_ = 0;
}

TableInformation::~TableInformation(void)
{
}

// TableContext accessor function for the ostream << operator
// usage: cout << (TableContext) mycontext << endl;
//
ostream& operator <<(ostream &os, const TableContext& context)
{
	os << "TABLE_CONTEXT: " << endl;
	os << "    common_pot: " << context.common_pot << endl;
	os << "    context.total_pot: " << context.total_pot << endl;
	os << "    bot_deal_position: " << context.bot_deal_position << endl;
	os << "    bot_bet_position: " << context.bot_bet_position << endl;
	os << "    num_players_dealt: " << context.num_players_dealt << endl;
	os << "    num_players_playing: " << context.num_players_playing << endl;
	os << "    num_players_behind: " << context.num_players_behind << endl;

	return os;
}

int TableInformation::IsBotHandConfirmed(void) const
{
	return (bot_hand_confirmed_ && table_context[index_].bot_cards.IsValid());
}

// Append the newest table context information to our stored history
// Returns 1 if we it is ok to run calculations on this update.
int TableInformation::UpdateTableContext(const TableContext& new_context)
{
	int			started_new_hand = 0;
	char		buf[64];

	Debug::log(LINFO) << "==== TableInformation::UpdateTableContext()" << std::endl;

	// Make sure we haven't accumulated 1024 context updates
	// during the current hand.  This _should_ be a good limit for now because
	// Limit Hold 'Em has max 10 players with max 4 betting levels and max 5 
	// betting rounds = 200 occurences.
	if (index_ == 1023) 
	{
		Debug::log(LERROR) << "ERROR: Reached upperlimit of class TableInformation" << std::endl;
		return 0;
	}

	// Log the updated information if we have a new hand
    // New hand is triggered by change in dealer_chair (button moves), or a change in hand_number
	if (new_context.hand_number != table_context[index_].hand_number ||
		new_context.dealer_chair != table_context[index_].dealer_chair)
	{
		if (table_context[index_].hand_number != 0)
		{
			// TODO: With OpenHoldem we can detect a new hand asap but the hand number is not sent
			// until it is our turn to play.  Man those guys suck.  So if we want to capture preflop
			// action we'll have to forgo knowing what the hand number is.

			Debug::log(LINFO) << "********************** NEW HAND **********************" << endl;
			Debug::log(LINFO) << new_context.table_name << endl;
			Debug::log(LINFO) << "Dealer: " << new_context.dealer_chair << endl << endl;

			// Determine whether we should reset the index to zero
			// when we save the table context later
			started_new_hand = 1;

			// Now we are not sure whether our read of the bot cards is
			// accurate and whether we should do calculations
			bot_hand_confirmed_ = 0;
		}
		else
		{
			// kludge b/c OpenHoldem delays sending new hand number and it fucks this up
			index_++;
			SaveCurrentTableContext(new_context);
			table_context[index_].hand_number = new_context.hand_number;
			sprintf_s(buf, sizeof(buf), "%.0f", new_context.hand_number);
			Debug::log(LINFO) << "==== HAND NUMBER: " << buf << endl << endl;
			return 0;  // Don't treat this as a valid update
		}
	}

    if (new_context.bot_cards.IsValid())
	{
		// Our bot has been dealt new cards.
		if (new_context.bot_cards[0] != table_context[index_].bot_cards[0] ||
			new_context.bot_cards[1] != table_context[index_].bot_cards[1])
		{
			Debug::log(LINFO) << "===== BOT CARDS: " << new_context.bot_cards << endl << endl;
			bot_hand_confirmed_ = 1;
		}
	}

	// We have a new betting round
	if (new_context.betting_round != table_context[index_].betting_round)
	{
		Debug::log(LINFO) << "===== NEW ROUND: " << new_context.betting_round 
						   << " Common cards: " << new_context.common_cards.flop1 << " " 
												<< new_context.common_cards.flop2 << " "
												<< new_context.common_cards.flop3 << " "
												<< new_context.common_cards.turn << " " 
												<< new_context.common_cards.river << endl << endl;
	}

	if (started_new_hand)
	{
		// Since we have a new hand we can dump the previous history
		// and start anew.
		index_ = 0;
	}
	else
	{	
		// Move the table context history iterator to the next 
		// available slot
		index_++;
	}

	// Log the new context
	Debug::log(LINFO) << "===== TABLE_CONTEXT: [" << index_ << "]" << endl;
	Debug::log(LINFO) << "common_pot: " << new_context.common_pot << endl;
	Debug::log(LINFO) << "context.total_pot: " << new_context.total_pot << endl;
	Debug::log(LINFO) << "bot_deal_position: " << new_context.bot_deal_position << endl;
	Debug::log(LINFO) << "bot_bet_position: " << new_context.bot_bet_position << endl;
	Debug::log(LINFO) << "num_players_dealt: " << new_context.num_players_dealt << endl;
	Debug::log(LINFO) << "num_players_playing: " << new_context.num_players_playing << endl;
	Debug::log(LINFO) << "num_players_behind: " << new_context.num_players_behind << endl;
	Debug::log(LINFO) << "bets_to_call: " << new_context.bets_to_call << endl;
	Debug::log(LINFO) << "bets_to_raise: " << new_context.bets_to_raise << endl;
	Debug::log(LINFO) << "current_bets: " << new_context.current_bets << endl << endl;

	// Now save the new table context
	SaveCurrentTableContext(new_context);

	return 1;	// We have a good update so it's safe to run calculations
}

void TableInformation::SaveCurrentTableContext(const TableContext& new_context)
{
	memcpy_s(table_context[index_].table_name, 64, new_context.table_name, 64);
	table_context[index_].hand_number = new_context.hand_number;

	table_context[index_].bot_cards = new_context.bot_cards;

	table_context[index_].common_cards.flop1 = new_context.common_cards.flop1;
	table_context[index_].common_cards.flop2 = new_context.common_cards.flop2;
	table_context[index_].common_cards.flop3 = new_context.common_cards.flop3;
	table_context[index_].common_cards.turn = new_context.common_cards.turn;
	table_context[index_].common_cards.river = new_context.common_cards.river;

	table_context[index_].betting_round = new_context.betting_round;

	table_context[index_].common_pot = new_context.common_pot;
	table_context[index_].total_pot = new_context.total_pot;

	table_context[index_].bot_chair = new_context.bot_chair;
	table_context[index_].dealer_chair = new_context.dealer_chair;

	table_context[index_].bets_to_call = new_context.bets_to_call;
	table_context[index_].bets_to_raise = new_context.bets_to_raise;
	table_context[index_].current_bets = new_context.current_bets;
}

TableContext* TableInformation::GetCurrentTableContext(void)
{
	return &table_context[index_];
}

// Returns true if the saved table context differs from the given context in a material way
int TableInformation::HasTableContextChanged(const TableContext& new_context)
{
	Debug::log(LDEBUG4) << "TableInformation::HasTableContextChanged(TableContext& new_context)" << std::endl;

	// If this is the first call since start-up return true.  Otherwise we'd never
	// determine that the context has changed.
	if (index_ == -1)
	{
		Debug::log(LINFO) << "FIRST PASS" << endl << endl;
		index_ = 0;
		return 1;
	}

	// NOTE: Need to verify that these are the only material changes we are
	// concerned about.
	// NOTE: Does not currently detect a player's check.

	if (table_context[index_].betting_round != new_context.betting_round) return 1;	// save context if new betting round
	if (table_context[index_].total_pot != new_context.total_pot) return 1;			// save context if anyone bets/raises
	if (table_context[index_].hand_number != new_context.hand_number) return 1;		// save context if new hand being dealt

	if (new_context.bot_cards.IsValid())
	{
		if (table_context[index_].bot_cards[0] != new_context.bot_cards[0] ||
			table_context[index_].bot_cards[1] != new_context.bot_cards[1]) return 1;
	}

	return 0;
}
