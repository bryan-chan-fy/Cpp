#include "Functions.h"
#include <cmath>
#include <complex>
#include <iostream>

using std::cout;
using std::endl;

// Define all functions

std::complex<double> SampleFunction1::operator() (double x, double Sigma, double Spot,double r, double delta, double Expiry, double v, double a)
{
    std::complex<double> f_x;
    double variance=Sigma*Sigma*Expiry;
    double mean=log(Spot)+(r-delta-Sigma*Sigma/2)*Expiry;
    double q_x;

    const std::complex<double> i(0,1);
    const double pi=std::acos(-1);

    q_x=(1/sqrt(variance*2*pi))*exp(-(x-mean)*(x-mean)/(2*variance));

    f_x=exp(i*(v-(a+1)*i)*x)*q_x;

    return f_x;
}

SampleFunction1::~SampleFunction1()
{
}

double delta_func(int j)
{
    if (j==0) {
        return 1.0;} else {
        return 0.0;}
}



