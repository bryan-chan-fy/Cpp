#ifndef writecsv_h
#define writecsv_h

#include <vector>
#include <string>

using std::vector;
using std::pair;
using std::string;

typedef vector<pair<string,vector<double>>> VectorPair;

void write_csv(string filename, VectorPair dataset);

#endif
