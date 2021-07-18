#ifndef readcsv_h
#define readcsv_h

#include <vector>
#include <string>

using std::vector;
using std::pair;
using std::string;

typedef vector<pair<string,vector<double>>> VectorPair;

VectorPair read_csv(string filename);

#endif