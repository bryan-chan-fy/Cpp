#ifndef BSMODEL_H
#define BSMODEL_H

#include <vector>

using std::vector;

typedef vector<double> SamplePath;

// Black Schole Model: Establish the Model Parameters and display Price/Greeks

class BSModel
{
public:

    BSModel(double S0_, double r_, double delta_, double Vol_);
    void GenerateSamplePath(double T,int N, SamplePath& S);                    // generate stock path
    void GenerateSamplePath(double T,int N, SamplePath& S1, SamplePath& S2);    // generate stock path for Antithetic
    double GetDelta(double T,double K,double St);
    void GetOptionPrice(double T,double K);

    double S0;
    double r;
    double delta;
    double Vol;

};

double normalCDF (double x);

double normalPDF (double x);

#endif
