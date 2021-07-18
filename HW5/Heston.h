#ifndef HESTON_H
#define HESTON_H

#include <vector>
#include <iostream>
#include <cmath>

#include "Random1.h"

using std::cout;
using std::endl;

using std::vector;

typedef vector<double> SamplePath;
typedef vector<double> VecArray;


// Heston Model: Establish the Model Parameters and Generate Path

class HSModel
{
public:

    HSModel () {}
    HSModel(double S0_, double V0_, double r_, double sigma_, double kappa_, double theta_, double rho_);
    virtual void GenerateSamplePath(double T,int N, SamplePath& S, SamplePath& V, int method);        // generate stock path
    virtual void GenerateSamplePath_Milhstein(double T,int N, SamplePath& S, SamplePath& V, int method);
    virtual void GenerateSampleTerminalPairs(double T,int l, int M, VecArray& SVec);
    virtual void GenerateEstimatorDebias(double T,int N, double K, double& Cn);
    virtual ~HSModel() {}

    double S0;
    double V0;
    double r;
    double sigma;
    double kappa;
    double theta;
    double rho;

};


double normalCDF (double x);

double normalPDF (double x);

#endif // HESTON
