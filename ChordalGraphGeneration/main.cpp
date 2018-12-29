#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include "Seeder.h"
#include "Chordal.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(a,b) _mkdir(a)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define SEED_FILE_NAME "seed.txt"
#define INSTANCE_INFO_FILE_NAME "instance_info.csv"
#define CLIQUE_SIZE_INFO_FILE_NAME	"clique_size_frequencies.csv"
#define DIRECTORY_SEPARATOR "/"

#define EPSILON 0.05

using namespace std;

int n;                      // Parameter #1: Number of vertices
double desiredDensity;      // Parameter #2: Desired density value
char *graphIndex;           // Parameter #3: The index of the graph
double densityMult; 	    // Parameter #4: Density multiplier, which is an optional parameter.
                            // The lower its value, the lower the expected density

static void readArguments(int argc, char ** argv) {
	if (argc < 4 || argc > 5) {
		cerr << "Usage: " << argv[0] << " number_of_vertices desired_density graph_index [density_multiplier]" << endl;
		exit(EXIT_FAILURE);
	}

	n = stoi(argv[1]);
	if (n < 1)
	{
		cout << "n should be a positive integer!" << endl;
		exit(EXIT_FAILURE);
	}

	desiredDensity = stod(argv[2]);
	if (desiredDensity > 1 || desiredDensity < 0)
	{
		cout << "Desired density should be between 0 and 1!" << endl;
		exit(EXIT_FAILURE);
	}

	graphIndex = argv[3];

	densityMult = 1;
	if (argc == 5)
	{
		densityMult = stod(argv[4]);
		if (densityMult > 1 || densityMult < 0)
		{
			cout << "Density control parameter should be between 0 and 1!" << endl;
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char ** argv) {

    readArguments(argc, argv);
	
	double densityEps = desiredDensity*EPSILON; //densityEps is the accepted maximum absolute deviation from the desired density value

	Seeder rp;
	rp.ReadWriteSeed(SEED_FILE_NAME);
	rp.seedRand();

	Chordal chg (n, densityMult);
	while (abs(chg.getDensity() - desiredDensity) > densityEps) {
		cerr << "Bad density: " << chg.getDensity() << " instead of " << desiredDensity << endl;
		chg.generate();
	}

	// WRITE THE CHORDAL GRAPH TO FILE
	string inputDir = to_string(n) + DIRECTORY_SEPARATOR;
	mkdir(inputDir.c_str(), 0777);
    inputDir = inputDir + "avg density " + argv[2] + DIRECTORY_SEPARATOR;
	mkdir(inputDir.c_str(), 0777);
	string fileName = inputDir + "chordalgr_" + argv[1] + "_" + graphIndex + ".txt";
	ofstream file;
	file.open(fileName, ios::out | ios::ate | ios::trunc);
    file << chg;
    file.close();

	chg.collectAndWriteStats(fileName, INSTANCE_INFO_FILE_NAME, CLIQUE_SIZE_INFO_FILE_NAME);
			
	cout << "n = " << n << endl;
	cout << "Density = " << chg.getDensity() << endl << endl;
}
