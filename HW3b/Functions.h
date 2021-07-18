#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <complex>


// Just an interface
class MathFunction
{
public:
    virtual std::complex<double> operator()(double x, double Sigma, double Spot,double r, double delta, double Expiry, double v, double a)=0;
    virtual ~MathFunction() {}
private:
};

class SampleFunction1: public MathFunction
{
public:
    virtual std::complex<double> operator()(double x, double Sigma, double Spot, double r,  double delta, double Expiry, double v, double a);
    virtual ~SampleFunction1();
};

double delta_func(int j);

#endif
