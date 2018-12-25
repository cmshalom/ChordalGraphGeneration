/*
 * NewSeeder.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: cmshalom
 */

#include "Seeder.h"

#include <fstream>
#include <random>

using namespace std;

void Seeder::ReadWriteSeed(string inputFile)
{
	fstream file;
	file.open(inputFile, std::fstream::in);
	file >> seed;
	file.close();
	file.open(inputFile, std::fstream::out | std::fstream::trunc);
	int nextSeed = seed + 1;
	file << nextSeed;
	file.close();
}

void Seeder::seedRand() {
	srand(seed);
}
