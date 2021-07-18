#ifndef BSMODEL_H
#define BSMODEL_H

#include <vector>

using std::vector;

typedef vector<double> SamplePath;


// Black Schole Model: Establish the Model Paramters and display Price/Greeks

class BSModel
{
public:
    double S0, r, delta, Vol;
    BSModel(double S0_, double r_, double delta_, double Vol_);
    void GenerateSamplePath(double T,int m, SamplePath& S); // dummy function
    void GetOptionPrice(double T,double K);
    void GetGreeks(double T,double K);
};

double normalCDF (double x);

double normalPDF (double x);

#endif
