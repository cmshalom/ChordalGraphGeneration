#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Seeder.h"
#include "Chordal.h"

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
		cerr << "Invalid number of arguments" << argc << endl;
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

	double densityMult = 1;
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

int main(int argc, char ** argv)
{
    readArguments(argc, argv);
	
	double densityEps = desiredDensity*EPSILON; //densityEps is the accepted maximum absolute deviation from the desired density value

	Seeder rp;
	rp.ReadWriteSeed(SEED_FILE_NAME);
	rp.seedRand();

	while (true) {
		Chordal chg(n, densityMult);
		if (abs(chg.getDensity() - desiredDensity) <= densityEps) {
			/************************************************************************************************/
			//TO WRITE THE CHORDAL GRAPH TO FILE
			string inputDir = to_string(n) + DIRECTORY_SEPARATOR;
			mkdir(inputDir.c_str(), 0777);
			inputDir = inputDir + "avg density " + argv[2] + DIRECTORY_SEPARATOR;
			mkdir(inputDir.c_str(), 0777);
			string fileName = inputDir + "chordalgr_" + argv[1] + "_" + graphIndex + ".txt";
			chg.writeGraphToFile(fileName);			
			/************************************************************************************************/

			/************************************************************************************************/
			//TO CALCULATE AND WRITE THE STATS OF THE CHORDAL GRAPH TO FILE
			chg.collectAndWriteStats(fileName, INSTANCE_INFO_FILE_NAME, CLIQUE_SIZE_INFO_FILE_NAME);
			/************************************************************************************************/
			
			cout << "n = " << chg.getNumberOfVertices() << endl;  // OYLUM n ile chg.n birbirinden farkli olabilir mi ?
			cout << "Density = " << chg.getDensity() << endl << endl;

			break;   // OYLUM: Bu nedir ilk basarili graphta program sona mi eriyor ?
		}
	}
}
