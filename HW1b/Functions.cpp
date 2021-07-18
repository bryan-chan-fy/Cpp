#include "Functions.h"
#include <cmath>

// Define all functions

double SampleFunction1::operator() (double x)
{
    double f_x;

    if (x!=0.0)
        f_x = sin(x)/x;
    else
        f_x = 1;
    return f_x;
}


double SampleFunction2D1::operator() (double x,double y)
{
    double f_x;

    return x*y*y;
}

double SampleFunction2D2::operator() (double x,double y)
{
    double f_x;

    return exp(2*x+y);
}

SampleFunction1::~SampleFunction1()
{
}

SampleFunction2D1::~SampleFunction2D1()
{
}

SampleFunction2D2::~SampleFunction2D2()
{
}