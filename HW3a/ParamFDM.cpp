#include "ParamFDM.h"
#include <cmath>

ParamFDM::ParamFDM(BSModel* PtrModel_, Option* PtrOption_):PtrModel(PtrModel_),PtrOption(PtrOption_)
{
    T=PtrOption->T;             // Getting the Stock Parameters
    K=PtrOption->K;
    isAmerican=PtrOption->isAmerican;

    Vol=PtrModel->Vol;
    r_int=PtrModel->r;
    S0=PtrModel->S0;
    delta=PtrModel->delta;
}

double ParamFDM::fT(double S)   // Getting PayOff
{
    return PtrOption-> PayOff(S);
}

double ParamFDM::fL_dCdS()      // Getting Lower Boundary dC/dS
{
    return PtrOption->LowerBound_dCdS();
}

double ParamFDM::fU_dCdS()      // Getting Upper Boundary dC/dS
{
    return PtrOption->UpperBound_dCdS();
}


