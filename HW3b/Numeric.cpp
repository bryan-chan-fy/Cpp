#include "Numeric.h"
#include <iostream>
#include <complex>

using std::cout;
using std::endl;


NumericImplement::NumericImplement(double t1, double t2, int n, MathFunction& f,double Sigma, double Spot,double r, double delta, double Expiry, double v, double a):T1(t1),T2(t2),N(n),F(f),SIGMA(Sigma),SPOT(Spot),R(r),DELTA(delta), EXPIRY(Expiry),V(v),A(a)
{   // nothing here
}

// Trapezoid method
std::complex<double> NumericImplement::Integral_Trapz() const
{
    double h=(T2-T1)/N;
    std::complex<double> y0,y1;
    double x0,x1;
    std::complex<double> sum=0.0;

    for (int i_=1;i_<=N;i_++)
    {
        x0=T1+(i_-1)*h;
        x1=T1+i_*h;

        y0 = F(x0,SIGMA,SPOT,R,DELTA,EXPIRY,V,A);
        y1 = F(x1,SIGMA,SPOT,R,DELTA,EXPIRY,V,A);
        sum += h*(y0+y1)/2.0;
    }
    return sum;
}

// Simpson Method
std::complex<double> NumericImplement::Integral_Simpson() const
{
    double h=(T2-T1)/N;
    std::complex<double> y0,y1,ym;
    double x0,x1,xm;
    std::complex<double> sum=0.0;

    for (int i_=1;i_<=N;i_++)
    {
        x0=T1+(i_-1)*h;
        x1=T1+i_*h;
        xm=(x0+x1)/2;
        y0 = F(x0,SIGMA,SPOT,R,DELTA, EXPIRY,V,A);
        ym = F(xm,SIGMA,SPOT,R,DELTA, EXPIRY,V,A);
        y1 = F(x1,SIGMA,SPOT,R,DELTA, EXPIRY,V,A);
        sum += (h/6.0)*(y0+4.0*ym+y1);
    }
    return sum;
}

