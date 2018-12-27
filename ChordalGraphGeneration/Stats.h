/*
 * Stats.h
 *
 *  Created on: Dec 27, 2018
 *      Author: cmshalom
 */

#ifndef STATS_H_
#define STATS_H_

#include <iostream>
#include <vector>

class Stats {
private:
	int lowerBound, upperBound;
	int min, max;
	long sum, count;
	long long sumSquares;
	std::vector<int> frequencies;
public:
	Stats(int lowerBound, int upperBound);
	void addSample(int);
	int getCount() const {return count;};
	int getSum() const {return sum;};
	int getMin() const {return min;};
	int getMax() const {return max;};
	double getAverage() const;
	double getVariance() const;
	double getStdDev() const;
	int getFrequency(int i) const {return frequencies[i];};
    friend	std::ostream& operator<<(std::ostream &, const Stats&);


	virtual ~Stats();
};

#endif /* STATS_H_ */
