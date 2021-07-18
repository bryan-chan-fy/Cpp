#include "Options.h"
#include <cmath>

// Return Options PayOff, Lower/Upper Boundary dC/dS

double Call::PayOff(double S)
{
    if (K>S) return 0.0;
    return S-K;
}

double Put::PayOff(double S)
{
    if (K<S) return 0.0;
    return K-S;
}

double Call::UpperBound_dCdS()
{
    return 1.0;
}

double Call::LowerBound_dCdS()
{
    return 0.0;
}

double Put::UpperBound_dCdS()
{
    return 0.0;
}

double Put::LowerBound_dCdS()
{
    return -1.0;
}
