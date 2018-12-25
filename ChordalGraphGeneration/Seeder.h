/*
 * NewSeeder.h
 *
 *  Created on: Dec 25, 2018
 *      Author: cmshalom
 */

#ifndef SEEDER_H_
#define SEEDER_H_

#include<string>

class Seeder {
private:
	int seed = 0;

public:
	void ReadWriteSeed(std::string);
	void seedRand();
};

#endif /* SEEDER_H_ */
