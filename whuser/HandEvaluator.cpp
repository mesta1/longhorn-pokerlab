#include "HandEvaluator.h"

#include <stdio.h>
#include <string.h>

HandEvaluator::HandEvaluator(void)
{
}

HandEvaluator::~HandEvaluator(void)
{
}

ProbabilityTriple HandEvaluator::GetPreflopAction(TableInformation* table)
{
	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}

ProbabilityTriple HandEvaluator::GetPostflopAction(TableInformation* table)
{
	ProbabilityTriple ptriple;
	SET_PTRIPLE(ptriple, 1, 0, 0);
	return ptriple;
}

int HandEvaluator::isHandInList(unsigned char c[2], const char* card_list)
{
	char strhand[4]; strhand[3] = NULL;
	char revstrhand[4]; revstrhand[3] = NULL;
	const char ranktochar[]="xA23456789TJQKA";

	// Convert hand from pokerval format to string
	strhand[0] = ranktochar[RANK(c[0])];
	strhand[1] = ranktochar[RANK(c[1])];
	if (RANK(c[0])==RANK(c[1]))
		strhand[2]=NULL;
	else
		ISSUITED(c[0],c[1]) ? strhand[2]='s' : strhand[2]=' ';
	
	// Make mirror image of string in case cards are in reverse order
	revstrhand[1] = strhand[0];
	revstrhand[0] = strhand[1];
	revstrhand[2] = strhand[2];

	if (strstr(card_list, strhand)) return 1; // found card in string list
	if (strstr(card_list, revstrhand)) return 1; // found card in string list
		
	return 0;  // found no match
}
