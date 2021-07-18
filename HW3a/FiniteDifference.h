#ifndef FINITEDIFFERENCE_H
#define FINITEDIFFERENCE_H

#include "ParamFDM.h"

#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

typedef vector<double> Vector;

class FiniteDifference
{
public:
    ParamFDM* PtrFDM;

    int N, M;
    double delta_x,delta_t,nu,Sigma,r,DX;
    double div;
    bool isAmerican;

    vector<Vector> C;   // 2D array for Option PayOff
    vector<Vector> S;   // 2D array for Stock Price
    vector<Vector> Q;   // for Crank Nicolson use

    FiniteDifference(ParamFDM* PtrFDM_, int N_, int M_, double DX);

    double fdT(double S);   // PayOff
    double fdL_dCdS();      // Lower Boundary dC/dS
    double fdU_dCdS();      // Upper Boundary dC/dS

    void ClearArray() { C.clear(); S.clear(); Q.clear(); }	// Clear some memory
    double getDelta();	// Get the Greeks
    double getGamma();
    double getTheta();
};

#endif
