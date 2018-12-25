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

class Chordal {
private:
	int numSubtrees, lastNodeID, numEdges, n;
	unsigned int numMaximalCliques, minCliqueSize, maxCliqueSize, numConnComps = 1;
	double densityMult, avgCliqueSize, stDevOfCliqueSizes;
	double timeToBuild;
	double density;

	std::vector<std::vector<int>> adjListG, adjListT, subtrees, subtreesContainingU;
	std::vector<int> sizeFrequenciesOfMaximalCliques;

	void generate(void);
	int newNode(void);

public:
	Chordal(int n, double densityMult = 1);
	void writeGraphToFile(std::string fileName);
	void collectAndWriteStats(std::string fileName, std::string instanceInfoCSVFileName, std::string cliqueSizeInfoCSVFileName);
	double getDensity() {return density;}
	int getNumberOfVertices() {return n;}
};

#endif /* CHORDAL_H_ */
