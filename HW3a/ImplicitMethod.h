#ifndef ImplicitMethod_h
#define ImplicitMethod_h

#include "FiniteDifference.h"
#include <iostream>

using std::cout;
using std::endl;

class ImplicitMethod: public FiniteDifference
{
public:
    ImplicitMethod(ParamFDM* PtrFDM_, int N_, int M_,double DX_): FiniteDifference(PtrFDM_,N_,M_,DX_){}

    double pu=-delta_t*(Sigma*Sigma/(2*delta_x*delta_x)+nu/(2*delta_x));
    double pm=1+delta_t*Sigma*Sigma/(delta_x*delta_x)+r*delta_t;
    double pd=-delta_t*(Sigma*Sigma/(2*delta_x*delta_x)-nu/(2*delta_x));

    double SolvePDE();
    void SolveTridiagonal(int index_);	
};

#endif
