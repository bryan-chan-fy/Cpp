#ifndef CrankNicolson_h
#define CrankNicolson_h

#include "FiniteDifference.h"
#include <iostream>

using std::cout;
using std::endl;

class CrankNicolson: public FiniteDifference
{
public:
    CrankNicolson(ParamFDM* PtrFDM_, int N_, int M_,double DX_): FiniteDifference(PtrFDM_,N_,M_,DX_){}

    double pu=-0.25*delta_t*(Sigma*Sigma/(delta_x*delta_x)+nu/delta_x);
    double pm=1+delta_t*Sigma*Sigma/(2*delta_x*delta_x)+r*delta_t/2;
    double pd=-0.25*delta_t*(Sigma*Sigma/(delta_x*delta_x)-nu/delta_x);

    double SolvePDE();
    void SolveTridiagonal(int index_);
};

#endif
