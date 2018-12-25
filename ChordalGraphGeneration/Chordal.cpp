/*
 * Chordal.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: cmshalom
 */

#include "Chordal.h"

#include <random>
#include <fstream>
#include <time.h>

using namespace std;

Chordal::Chordal(int n, double densityMult) {
	this->n = n;
	this->densityMult = densityMult;
	generate();
}


void Chordal::generate(void) {
	clock_t start = clock();
	lastNodeID = -1;
	numSubtrees = 0;
	adjListG.resize(n);
	subtrees.resize(n);
	adjListT.reserve(n);
	subtreesContainingU.reserve(n);
	sizeFrequenciesOfMaximalCliques.reserve(n);

	int numNewSubtrees = newNode();
	while (numSubtrees < n) {
		int selectedNodeID = rand() % adjListT.size(); //selected node is node u
		numNewSubtrees = newNode();                    //new node is node v
		adjListT.back().emplace_back(selectedNodeID);
		adjListT.at(selectedNodeID).emplace_back(lastNodeID);
		//select a proper subset
		vector<int> subtreeIDsInProperSubset;
		subtreeIDsInProperSubset.reserve(subtreesContainingU.at(selectedNodeID).size());
		while (true) {
			subtreeIDsInProperSubset.clear();
			for (unsigned int i = 0; i < subtreesContainingU.at(selectedNodeID).size(); ++i) {
				double randomNo = double(rand()) / RAND_MAX;
				if (randomNo < 0.5)
					subtreeIDsInProperSubset.emplace_back(subtreesContainingU.at(selectedNodeID).at(i));
			}
			if (subtreeIDsInProperSubset.size() < subtreesContainingU.at(selectedNodeID).size() - 0.5)
				break;
		}
		int properSubsetSize = subtreeIDsInProperSubset.size();
		if (properSubsetSize < 0.1) //if the selected proper subset is empty
			numConnComps++;
		for (vector<int>::iterator it = subtreeIDsInProperSubset.begin(); it!= subtreeIDsInProperSubset.end(); ++it) {
			int subtreeID = *it;
			subtrees.at(subtreeID).emplace_back(lastNodeID);
			subtreesContainingU.back().emplace_back(subtreeID);
			for (int j = numSubtrees - numNewSubtrees; j < numSubtrees; ++j) {
				adjListG.at(subtreeID).emplace_back(j);
				adjListG.at(j).emplace_back(subtreeID);
			}
		}
		numEdges += numNewSubtrees*properSubsetSize;
	}
	density = double(numEdges) / (n*(n - 1) / 2);
	clock_t end = clock();
	timeToBuild = double(end - start) / 1000;
}

int Chordal::newNode(void) {
	lastNodeID++;
	adjListT.resize(lastNodeID + 1);
	/*
	//either pick an integer between 1 and n-_numSubtrees
	int k = rand() % (_n - _numSubtrees) + 1;
	*/
	//or decrease the upper bound to guide the density (when _densityMult=1, this becomes equivalent to the one above)
	int k = rand() % (max(1, int( (n - numSubtrees)*this->densityMult + 0.5 ))) + 1;
	for (int i = numSubtrees; i < numSubtrees + k; ++i) subtrees.at(i).emplace_back(lastNodeID);
	for (int i = numSubtrees; i < numSubtrees + k; ++i) {
		for (int j = numSubtrees; j < numSubtrees + k; ++j) {
			if (i != j)	adjListG.at(i).emplace_back(j);
		}
	}
	numEdges += k*(k - 1) / 2;

	//update the set of subtrees containing u
	int currentSize = subtreesContainingU.size();
	subtreesContainingU.resize(currentSize + 1); //last index of _subtreesContainingU corresponds to the new node v
	for (int i = numSubtrees; i < numSubtrees + k; ++i)
		subtreesContainingU.back().emplace_back(i);
	numSubtrees += k;
	return k;
}

void Chordal::writeGraphToFile(string fileName) {
	ofstream file;
	file.open(fileName, ios::out | ios::ate | ios::trunc);
	file << n << endl;
	for (unsigned int i = 0; i < adjListG.size(); ++i) {
		file << adjListG.at(i).size() << " ";
		for (unsigned int j = 0; j < adjListG.at(i).size(); ++j) file << adjListG.at(i).at(j) << " ";
		file << endl;
	}
}

void Chordal::collectAndWriteStats(string fileName, string instanceInfoCSVFileName, string cliqueSizeInfoCSVFileName) {
	//collect stats
	numMaximalCliques = subtreesContainingU.size();
	long long int sumSquaredSizes = 0;
	minCliqueSize = n + 1; maxCliqueSize = 0; avgCliqueSize = 0;
	for (vector<vector<int>>::const_iterator cit = subtreesContainingU.begin(); cit != subtreesContainingU.end(); ++cit) {
		unsigned int cliqueSize = (*cit).size();
		avgCliqueSize += cliqueSize;
		if (cliqueSize < minCliqueSize) minCliqueSize = cliqueSize;
		if (cliqueSize > maxCliqueSize) maxCliqueSize = cliqueSize;
		sumSquaredSizes += cliqueSize*cliqueSize;
		if (sizeFrequenciesOfMaximalCliques.size() < cliqueSize) {
			sizeFrequenciesOfMaximalCliques.resize(cliqueSize); // so that (cliqueSize-1)^th entry is guaranteed to exist (resize equates the newly added entries to zero)
		}
		sizeFrequenciesOfMaximalCliques.at(cliqueSize-1)++;
	}
	avgCliqueSize = double(avgCliqueSize) / numMaximalCliques;
	double variance = (1 / double((numMaximalCliques - 1)))*(sumSquaredSizes - numMaximalCliques*avgCliqueSize*avgCliqueSize);
	stDevOfCliqueSizes = sqrt(variance);

	//write stats to file
	ofstream file;
	file.open(instanceInfoCSVFileName, ios::out | ios::ate | ios::app);
	file << fileName << "," << n << "," << density << "," << this->densityMult << ","
		<< numConnComps << "," << numMaximalCliques << "," << minCliqueSize << "," << maxCliqueSize << "," << avgCliqueSize << "," << stDevOfCliqueSizes << ","
		<< timeToBuild << endl;
	ofstream file2;
	file2.open(cliqueSizeInfoCSVFileName, ios::out | ios::ate | ios::app);
	for (unsigned int i = 0; i < sizeFrequenciesOfMaximalCliques.size(); ++i) {
		file2 << sizeFrequenciesOfMaximalCliques.at(i) << ",";
	}
	file2 << endl;
}
