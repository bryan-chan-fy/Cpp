#include "writecsv.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using std::vector;
using std::pair;
using std::string;
using std::cout;
using std::endl;

typedef vector<pair<string,vector<double>>> VectorPair;

void write_csv(string filename, VectorPair dataset){
    // Write a CSV file with vector of <string, vector<double> pairs - called datasets
    // each pair represents <column name, column values>
    // all columns should be the same size

    // output filestream
    std::ofstream myFile(filename);

    // write column names to the stream
    for (int j=0;j<dataset.size();j++){
        myFile << dataset.at(j).first;
        if (j!=dataset.size()-1) myFile << ","; // add comma separator
    }
    myFile << "\n";

    // write column data to the stream
    for (int i=0; i<dataset.at(0).second.size();i++){
        for (int j=0;j<dataset.size();j++){
            myFile << dataset.at(j).second.at(i);
            if(j!=dataset.size()-1) myFile << ","; // add comma separator
        }
        myFile << "\n";
    }

    // Close the file
    myFile.close();
}

