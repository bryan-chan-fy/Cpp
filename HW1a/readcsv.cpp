#include "readcsv.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using std::vector;
using std::pair;
using std::string;
using std::cout;
using std::endl;

typedef vector<pair<string,vector<double>>> VectorPair;

VectorPair read_csv(string filename){
    // Read a CSV file into vector of <string, vector<double>> pairs - called result
    // each pair represents <column name, column values>

    vector<pair<string,vector<double>>> result;

    // input filestream
    std::ifstream myFile(filename);

    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    string line,colname;
    double val;

    if(myFile.good())
    {
        // extract first line for column names
        std::getline(myFile,line);
        std::stringstream ss(line);

        while(std::getline(ss,colname,','))
        {
            colname.erase(std::remove(colname.begin(),colname.end(),'\"'),colname.end()); // remove quotes
            result.push_back({colname,std::vector<double>{}});
        }
    }

    while(std::getline(myFile,line))
    {
        // extract 2nd lines onwards for values
        std::stringstream ss(line);
        int colIndex=0;

        while(ss>>val){
            result.at(colIndex).second.push_back(val);
            if (ss.peek()==',') ss.ignore();  // ignore if next token is comma
            colIndex++;
        }
    }

    myFile.close();
    return result;

}