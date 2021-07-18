#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "readcsv.h"
#include "writecsv.h"
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::pair;
using std::vector;

typedef vector<pair<string,vector<double>>> VectorPair;

class Parameters
{
public:
    Parameters() {};
    double GetValue(string filename,string searchstr,int whichrow);
    void WriteValue(string filename,VectorPair dataset);
    ~Parameters() {};
private:
};

#endif
