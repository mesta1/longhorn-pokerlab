#pragma once

#ifndef _OPPONENTMODEL_H_
#define _OPPONENTMODEL_H_

class OpponentModel
{
public:
	OpponentModel(void);
	~OpponentModel(void);

private:

	// p_dealtcards
	// Array of probabilities that the player would hold any given card.  The array is
	// in the order (hearts, diamonds, clubs, spades) because that is the order
	// defined by Global.h
	// e.g., p_cards[0 - 5...] = [Ah Ad Ac As Kh Kd ...]
	double p_cards[52];

	// p_holecards
	// Graph of probabilities that the play holds any two unique holecards. This
	// is defined by P(c1|c2) where:
	//
	// P(c1) = P(c1h)+ P(c1d) + P(c1c) + P(c1s)
	//
	// This is not useful for calculations, just for graphical display.  Array is
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
};

#endif