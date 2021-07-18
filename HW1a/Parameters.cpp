#include "Parameters.h"

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::cout;
using std::vector;
using std::pair;
using std::endl;

typedef vector<pair<string,vector<double>>> VectorPair;

double Parameters::GetValue(string filename,string searchstr,int whichrow)
{
    VectorPair resultvec;
    double foundvalue;

    // read csv into result vector
    resultvec = read_csv(filename);

    cout << searchstr <<endl;

    // find the corresponding column that matches column search string and its value at row
    for (int i=0;i<resultvec.size();i++){
        if (resultvec.at(i).first==searchstr) {
            foundvalue = resultvec.at(i).second.at(whichrow);
            cout << foundvalue << endl;
        }
    }

    return foundvalue;
}

// Write values by calling Write CSV file
void Parameters::WriteValue(string filename,VectorPair dataset){

    write_csv(filename,dataset);
}