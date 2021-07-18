#include "Compute.h"
#include "Numeric.h"
#include <iostream>

using std::cout;
using std::endl;

// Compute (Integration) together with truncation error based on different Method
double Compute(const Numeric& Area, MethodType Method)
{
    double result=0.0;

    switch (Method)
    {
    case Trapezoid:
        result=Area.Integral_Trapz();
        break;
    case Simpson:
        result=Area.Integral_Simpson();
        break;
    case Double:
        result=Area.Double_Integral();
        break;
    }

    return result;
}

// calculation Trunc error for Trapezoid Method
double TruncError_Trapezoid (double a,int n, MathFunction& f, double Target, double& int_result)
{
    double diff=0.0;

    NumericImplement Area(-a,a,n,f);  // setup t1,t2, n and Function

    int_result=Compute(Area,Trapezoid);
    diff=int_result-Target;

    return diff;
}

// calculation Trunc error for Simpson Method
double TruncError_Simpson (double a,int n, MathFunction& f, double Target, double& int_result)
{
    double diff=0.0;

    NumericImplement Area(-a,a,n,f);  // setup t1,t2, n and Function

    int_result=Compute(Area,Simpson);
    diff=int_result-Target;

    return diff;
}

// calculation Trunc error for Double Integration Method
double TruncError_Double_Integral (double a,double b,int n, double c,double d, int m, MathFunction& f, double Target, double& int_result)
{
    double diff=0.0;

    NumericImplement Area(a,b,n,c,d,m,f);  // setup t1,t2, n, t3, t4, m and Function

    int_result=Compute(Area,Double);
    diff=int_result-Target;

    return diff;
}
