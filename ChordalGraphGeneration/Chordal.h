/*
 * Chordal.h
 *
 *  Created on: Dec 25, 2018
 *      Author: cmshalom
 */

#ifndef CHORDAL_H_
#define CHORDAL_H_

#include <vector>
#include <string>
#include <iostream>

#include "Stats.h"

class Chordal {
private:
	// Data initialized by c'tors
	unsigned int n;
	double densityMult;

	// Data computed by generate()
	int lastNodeID;
	unsigned int numSubtrees, numEdges, numConnComps;
	double timeToBuild;
	std::vector<std::vector<int>> adjListG, subtreesContaining;
	double density;

	int newNode(void);
	void addTreeEdge(int parent, int child);
	int numberOfNewSubtrees(void);


public:
	Chordal(int n, double densityMult = 1);
	void generate(void);
	void writeGraphToFile(std::string fileName);
	void collectAndWriteStats(std::string fileName, std::string instanceInfoCSVFileName, std::string cliqueSizeInfoCSVFileName);
	double getDensity() {return density;}
	int getNumberOfVertices() {return n;}
	Stats *getCliqueStatistics();

    friend	std::ostream& operator<<(std::ostream &, const Chordal&);

};

#endif /* CHORDAL_H_ */
