#include "BSModel.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>

using std::vector;
using std::cout;
using std::endl;

typedef vector<double> SamplePath;

const double pi=4.0*atan(1.0);

BSModel::BSModel(double S0_,double r_,double delta_,double Vol_):S0(S0_),r(r_), delta(delta_), Vol(Vol_)
{
}

void BSModel::GetOptionPrice(double T,double K)
{
    // Display Option Prices
    double d1;
    double d2;

    d1 =(log(S0/K) + (r-delta+Vol*Vol/2)*T)/(Vol*sqrt(T));
    d2 = d1 - Vol*sqrt(T);

    cout << " Call Price: " << S0*exp(-delta*T)*normalCDF(d1)-K*exp(-r*T)*normalCDF(d2) << endl;
    cout << " Put Price: " << K*exp(-r*T)*normalCDF(-d2)-S0*exp(-delta*T)*normalCDF(-d1) << endl;

}

void BSModel::GetGreeks(double T,double K)
{
    // Display Greeks
    double d1;
    double d2;

    d1 =(log(S0/K) + (r-delta+Vol*Vol/2)*T)/(Vol*sqrt(T));
    d2 = d1 - Vol*sqrt(T);

    cout << " Delta: " << exp(-delta*T)*normalCDF(d1) << endl;
    cout << " Gamma: " << normalPDF(d1)*exp(-delta*T)/(S0*Vol*sqrt(T)) << endl;
    cout << " Theta: " << -S0*normalPDF(d1)*Vol*exp(-delta*T)/(2*sqrt(T))+delta*S0*normalCDF(d1)*exp(-delta*T)-r*K*exp(-r*T)*normalCDF(d2) << endl;
    cout << " Vega: " << S0*sqrt(T)*normalPDF(d1)*exp(-delta*T) << endl;

}


double normalCDF (double x) {
    return 0.5* std::erfc(-x/sqrt(2));
}

double normalPDF(double x){
    return 1/sqrt(2*pi)*exp(-x*x/2);
}
