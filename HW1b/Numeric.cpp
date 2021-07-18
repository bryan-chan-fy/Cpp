#include "Numeric.h"
#include <iostream>

using std::cout;
using std::endl;

Numeric::Numeric(const NumericInner& inner_Object)
{
    InnerObjectPtr = inner_Object.clone();
}

Numeric::Numeric(const Numeric& original)
{
    InnerObjectPtr = original.InnerObjectPtr->clone();
}

Numeric& Numeric::operator=(const Numeric& original)
{
    if (this != &original)        // check against self-assignment A=A
        delete InnerObjectPtr;
        InnerObjectPtr = original.InnerObjectPtr->clone();
}

Numeric::~Numeric()
{
    delete InnerObjectPtr;
}

NumericInner* NumericImplement::clone() const
{
    return new NumericImplement(T1,T2,N,T3,T4,M,F);
    return new NumericImplement(T1,T2,N,F);
}

NumericImplement::NumericImplement(double t1, double t2, int n, MathFunction& f):T1(t1),T2(t2),N(n),F(f)
{   // nothing here
}

NumericImplement::NumericImplement(double t1, double t2, int n, double t3, double t4, int m, MathFunction& f):T1(t1),T2(t2),N(n),T3(t3),T4(t4), M(m),F(f)
{   // nothing here
}



// Trapezoid method
double NumericImplement::Integral_Trapz() const
{
    double h=(T2-T1)/N;
    double y0,y1;
    double x0,x1;
    double sum=0.0;

    for (int i=1;i<=N;i++)
    {
        x0=T1+(i-1)*h;
        x1=T1+i*h;
        y0 = F(x0);
        y1 = F(x1);
        sum += h*(y0+y1)/2;
    }
    return sum;
}

// Simpson Method
double NumericImplement::Integral_Simpson() const
{
    double h=(T2-T1)/N;
    double y0,y1,ym;
    double x0,x1,xm;
    double sum=0.0;

    for (int i=1;i<=N;i++)
    {
        x0=T1+(i-1)*h;
        x1=T1+i*h;
        xm=(x0+x1)/2;
        y0 = F(x0);
        ym = F(xm);
        y1 = F(x1);
        sum += (h/6)*(y0+4*ym+y1);
    }
    return sum;
}

// Double Integral
double NumericImplement::Double_Integral() const
{
    double hx=(T2-T1)/N;
    double hy=(T4-T3)/M;
    double y0,y1;
    double x0,x1;
    double sum=0.0;

    for (int i=1;i<=N;i++)
    {
        x0=T1+(i-1)*hx;
        x1=T1+i*hx;

        for (int j=1;j<=M;j++)
        {
            y0=T3+(j-1)*hy;
            y1=T3+j*hy;

            sum += (hx*hy/16)*(F(x0,y0)+F(x0,y1)+F(x1,y0)+F(x1,y1)+2*(F(0.5*(x0+x1),y0) + F(0.5*(x0+x1),y1)+F(x0,0.5*(y0+y1)) +F(x1,0.5*(y0+y1))) + 4*(F(0.5*(x0+x1),0.5*(y0+y1))));
        }
    }
    return sum;
}

