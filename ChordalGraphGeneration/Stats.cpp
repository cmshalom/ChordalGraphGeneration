/*
 * Stats.cpp
 *
 *  Created on: Dec 27, 2018
 *      Author: cmshalom
 */

#include "Stats.h"
#include <cmath>

using namespace std;

Stats::Stats(int lowerBound, int upperBound) {
	this->lowerBound = lowerBound;
	this->upperBound = upperBound;
	min = upperBound;
	max = lowerBound;
	sum = 0;
	count = 0;
	sumSquares = 0;
	frequencies.reserve(lowerBound-upperBound+1);
}

Stats::~Stats() {
}

void Stats::addSample(int sample) {
    if (sample < min) min = sample;
    if (sample > max) {
    	max = sample;
    	frequencies.resize(max-lowerBound+1);
    }
    count++;
    sum += sample;
    sumSquares += sample * sample;
	frequencies[sample-lowerBound]++;
}

double Stats::getAverage() const {
	return double(sum) / count;
}

double Stats::getVariance() const {
	double avg = getAverage();
	return (sumSquares - count*avg*avg)/count;
	// OYLUM: Variance formulunu konusalim
}

double Stats::getStdDev() const {
	return std::sqrt(getVariance());
}

ostream& operator<<(ostream &out, const Stats& stats) {
	out << stats.count << "," << stats.min << "," << stats.max << "," << stats.getAverage() << "," << stats.getStdDev();
	return out;
}
