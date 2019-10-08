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

#ifdef _WIN32
#include <windows.h>
#endif


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
	numEdges = 0;
	numConnComps = 1;
	adjListG.clear();
	subtreesContaining.clear();
	adjListG.resize(n);
	subtreesContaining.reserve(n);

	newNode();
	while (numSubtrees < n) {
		int u = rand() % (lastNodeID+1);
		newNode();
		addTreeEdge(u, lastNodeID);
	}
	density = double(numEdges) / (n*(n - 1) / 2);

	clock_t end = clock();
	timeToBuild = double(end - start) / 1000;
}

int Chordal::newNode(void) {
	lastNodeID++;
	// Note that when densityMult=1 the below becomes
	// int k = rand() % (n - numSubtrees) + 1;
	int k = rand() % (max(1, int( (n - numSubtrees)*densityMult + 0.5 ))) + 1;

	//update the set of subtrees containing u
	subtreesContaining.resize(lastNodeID+1);
	for (int i = 0; i < k; i++)
		subtreesContaining.back().emplace_back(numSubtrees + i);

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			if (i != j)	adjListG[numSubtrees + i].emplace_back(numSubtrees+j);
		}
	}
	numEdges += k*(k - 1) / 2;

	numSubtrees += k;
	return k;
}

void Chordal::addTreeEdge(int u, int v) {
	// Select a proper subset of the subtrees of u
	vector<int> properSubset;
	properSubset.reserve(subtreesContaining[u].size());
	do {
		properSubset.clear();
		for (unsigned int i = 0; i < subtreesContaining[u].size(); i++) {
			if (rand() % 2 == 0) properSubset.emplace_back(subtreesContaining[u][i]);
		}

	} while (properSubset.size() == subtreesContaining[u].size());

	if (properSubset.size() == 0) numConnComps++;

	// Extend every subtree of the proper subset to v
	int vSubtrees = subtreesContaining[v].size();
	for (unsigned int i = 0; i < properSubset.size(); i++) {
		int treeFromU = properSubset[i];
		subtreesContaining[v].push_back(treeFromU);
		for (unsigned int treeFromV = numSubtrees - vSubtrees; treeFromV < numSubtrees; treeFromV++) {
			adjListG[treeFromU].emplace_back(treeFromV);
			adjListG[treeFromV].emplace_back(treeFromU);
		}
	}
	numEdges += vSubtrees*properSubset.size();
}

ostream& operator<<(ostream &out, const Chordal& chordal) {
	out << chordal.n << endl;
	for (unsigned int i = 0; i < chordal.adjListG.size(); i++) {
		out << chordal.adjListG[i].size() << " ";
		for (unsigned int j = 0; j < chordal.adjListG[i].size(); j++) out << chordal.adjListG[i][j] << " ";
		out << endl;
	}
	return out;
}

Stats *Chordal::getCliqueStatistics() {
	Stats *ret = new Stats(1,n);

	for (unsigned int v = 0; v < subtreesContaining.size(); v++) {
		ret->addSample(subtreesContaining[v].size());
	}
    return ret;
}


void Chordal::collectAndWriteStats(string fileName, string instanceInfoCSVFileName, string cliqueSizeInfoCSVFileName) {
	Stats *stats = getCliqueStatistics();

	ofstream file;
	file.open(instanceInfoCSVFileName, ios::out | ios::ate | ios::app);

	file << fileName << "," << n << "," << density << "," << densityMult << ","
		<< numConnComps << "," << *stats << "," << timeToBuild << endl;
	file.close();

	ofstream file2;
	file2.open(cliqueSizeInfoCSVFileName, ios::out | ios::ate | ios::app);
	for (int i = 0; i < stats->getMax(); i++) {
		file2 << stats->getFrequency(i) << ",";
	}
	file2 << endl;
	file2.close();
	delete stats;
}
