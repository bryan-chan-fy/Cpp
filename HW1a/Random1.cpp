#include "Random1.h"
#include <cstdlib>
#include <cmath>

#define pi 4.0*atan(1.0)

double GetBoxMullerGaussian()
{
    double U1;
    double U2;

    U1 = (rand()+1.0)/(RAND_MAX+1.0);
    U2 = (rand()+1.0)/(RAND_MAX+1.0);

    return sqrt(-2.0*log(U1))*cos(2.0*pi*U2);

}