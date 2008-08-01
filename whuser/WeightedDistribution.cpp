
#include "WeightedDistribution.h"

#include <stdlib.h>
#include <time.h>

WeightedDistribution::WeightedDistribution(void)
{
	weight_table = 0;
	mask_table = 0;
	weight_total = 0.0;
	mask_total = 0.0;
}

WeightedDistribution::WeightedDistribution(int size)
{
	int i;

	// Seed our random number generator
	srand((unsigned)time(0));

	// Create a new table with the given size
	weight_table = new double[size];
	weight_total = 0.0;
	mask_total = 0.0;
	table_size = size;

	// Create a new mask table with the given size
	mask_table = new double[size];

	for (i=0; i < table_size; i++)
	{
		// Set the default weight to 1.0 (equal distribution)
		weight_table[i] = 1.0;
		weight_total += 1.0;

		// Set the default mask value to 100% opacity(unmasked)
		mask_table[i] = 1.0;
		mask_total += 1.0;
	}
}

WeightedDistribution::~WeightedDistribution(void)
{
	if (weight_table)
	{
		delete weight_table;
		weight_table = 0;
	}
}

// Gets a random number from (0-table_size) using weighted distribution.
// This function also allows for a mask which alters each element's weight.
// Mask values of 1.0 (100%) retain the original weight while mask values
// of 0.0 (0%) change the weight to zero.
//
// The mask allows you to non-destrictively test random distributions
// under different scenarios.
int WeightedDistribution::rand()
{
	double random = 0, alpha = 0, sum = 0;
    int index;

    // Generate random number from 0 to the sum total (masked) weight
	// We used the masked total, which is a running sum of the
	// weights as they are masked.
	random = alpha * (::rand())/(RAND_MAX + 1.0);

	// Run through the weight table and stop once we reach the
	// random number.
	for (index=0; index < table_size; index++)
	{
		// If we've reached the weight value below which our random number
		// falls, our index now is equal desired weighted random value and
		// we can return.
		if (random < weight_table[index] * mask_table[index]) break;

		// Otherwise, iterate past this masked weight value and repeat.
		random -= weight_table[index] * mask_table[index];
	}

	return (index);
}

// set the weight at an index
void WeightedDistribution::SetWeight(const int index, const double weight)		
{
	if ((index >= 0) && (index < table_size))
	{
		// Remove the original weight from our totals
		weight_total -= weight_table[index];
		mask_total -= weight_table[index] * mask_table[index];

		// Set the desired weight
		weight_table[index] = weight;

		// Add the new weight to our totals
		weight_total += weight;
		mask_total += weight * mask_table[index];
	}
	else
	{
		//ERROR
		return;
	}
}

// get the weight at an index
double WeightedDistribution::GetWeight(int index) const		
{
	if ((index >= 0) && (index < table_size))
	{
		return (weight_table[index]);
	}
	else
	{
		//ERROR
		return 0.0;
	}
}

double WeightedDistribution::operator[](int index) const
{
	if ((index >= 0) && (index < table_size))
	{
		return (weight_table[index]);
	}
	else
	{
		//ERROR
		return 0.0;
	}
}

// get the normalized weight (0-1) at an index
double WeightedDistribution::GetNormalized(int index) const	
{
	if ((index >= 0) && (index < table_size))
	{
		return (weight_table[index] / weight_total);
	}
	else
	{
		//ERROR
		return 0.0;
	}
}

// reevaluate all weights so that weight_total=1;
void WeightedDistribution::Normalize(void)	
{
	int i;

	for (i=0; i < table_size; i++)
	{
		weight_table[i] = weight_table[i] / weight_total;
	}
	
	weight_total = 1.0;
}

void WeightedDistribution::SetMask(const int index, const double mask_value)
{
	if ((index >= 0) && (index < table_size))
	{
		mask_total -= weight_table[index] * mask_table[index];
		mask_table[index] = mask_value;
		mask_total += weight_table[index] * mask_value;
	}
	else
	{
		//ERROR
		return;
	}

}

void WeightedDistribution::ClearMask(void)
{
	int i;

	for (i=0; i < table_size; i++)
	{
		// Set the mask value to 100% opacity (unmasked)
		mask_total -= weight_table[i] * mask_table[i];
		mask_table[i] = 1.0;
		mask_total -= weight_table[i] * 1.0;
	}

}