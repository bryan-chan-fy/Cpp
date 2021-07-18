#ifndef NUMERIC_H
#define NUMERIC_H

#include "Functions.h"

#include <complex>


// Implementation
class NumericImplement
{
public:
    NumericImplement(double t1, double t2, int n, MathFunction& f, double Sigma, double Spot,double r, double delta, double Expiry, double v, double a);
    virtual std::complex<double> Integral_Trapz() const;
    virtual std::complex<double> Integral_Simpson() const;
private:
    double T1;
    double T2;
    double N;
    double SIGMA;
    double SPOT;
    double R;
    double EXPIRY;
    double V;
    double A;
    double DELTA;

    MathFunction& F;
};

#endif
