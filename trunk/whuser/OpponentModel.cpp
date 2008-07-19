#include "OpponentModel.h"

OpponentModel::OpponentModel(void)
{
	// Initialize our probability chart
	for (int i=0; i < 52; i++) p_cards[i] = (double) (1/52);

	// Each time we initialize our probabiliy chart
	// we must update the hole card graph
	UpdateHoleCardGraph();

}

OpponentModel::~OpponentModel(void)
{
}

void OpponentModel::UpdateHoleCardGraph(void)
{
	//TODO: update p_holecard[][] array.
}
