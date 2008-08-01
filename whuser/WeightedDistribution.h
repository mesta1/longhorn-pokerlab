#pragma once

#include <iostream>
using namespace std;

class WeightedDistribution
{
public:
	WeightedDistribution(int);			// constructor which creates a table of a given size

	int rand();							// get a random integer from (0-table_size) using weight distribution

	void SetWeight(const int, const double);	// set the weight at an index
	double GetWeight(int) const;		// get the weight at an index
	double GetNormalized(int) const;	// get the normalized weight at an index

	void SetMask(const int, const double);	// non-destructively set the opacity at an index
	void ClearMask(void);				// clear the entire mask (set opacity to 100%)

	void Normalize(void);				// reevaluate all weights so that weight_total=1;
	
	void Serialize(std::ostream&);		// convert weight table to byte stream;
	int Unserialize(std::istream&);		// read weight table from byte stream;

	double operator[](int) const;	// get the weight at an index
	WeightedDistribution& operator[](int);	// set the weight at an index

public:
	WeightedDistribution(void);		// default constructor
	~WeightedDistribution(void);
	
	// Right now the distribution is stored as an array.
	// Since the rand() member function operates on an
	// array in O(n/2) time on average, this would be better
	// implemented as a binomia
	double*			weight_table;
	double*			mask_table;

	double			weight_total;
	double			mask_total;

	int				table_size;
};
