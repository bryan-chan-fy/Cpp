#ifndef ParamFDM_H
#define ParamFDM_H

#include "BSModel.h"
#include "Options.h"

class ParamFDM              // Handle the parameters
{
public:
    BSModel* PtrModel;      // Pointers to BSModel
    Option* PtrOption;      // Pointers to Option
    bool isAmerican;

    ParamFDM(BSModel* PtrModel_, Option* PtrOption_);

    double T,r_int,delta,Vol,S0,K;  // Stock Parameters

    double fT(double S);	// PayOff
    double fL_dCdS();		// Lower Boundary dC/dS
    double fU_dCdS();		// Upper Boundary dC/dS
};

#endif
