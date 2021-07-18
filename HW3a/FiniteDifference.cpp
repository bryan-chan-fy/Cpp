#include "FiniteDifference.h"

#include <cmath>
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;



FiniteDifference::FiniteDifference(ParamFDM* PtrFDM_,int N_, int M_,double DX_): PtrFDM(PtrFDM_),N(N_),M(M_),DX(DX_)
{
    double Expiry = PtrFDM->T;		// Transfer parameters from pointers
    double div = PtrFDM->delta;
    double Spot = PtrFDM->S0;
    isAmerican = PtrFDM-> isAmerican;

    r = PtrFDM->r_int;
    Sigma = PtrFDM->Vol;
    delta_t= Expiry/N;
    nu=r - div - Sigma*Sigma/2;
    if (DX==0.0) { delta_x= Sigma*sqrt(3*delta_t);  // delta_x = DX if provided
    } else {delta_x=DX;}

    C.resize(N+1);    // N steps
    S.resize(N+1);
    Q.resize(N+1);      // For Crank-Nicolson

    for (int i=0;i<=N;i++)
    {
        C[i].resize(2*M+1);
        S[i].resize(2*M+1);
        Q[i].resize(2*M+1);
    }

    // initialize asset prices at maturity and other times
    for (int i=0;i<=N;i++)
    {
        for (int j=0; j<=2*M; j++)
        {
            S[i][j]=Spot*exp(-M*delta_x + j*delta_x);    // because x=ln S
        }
    }
}

double FiniteDifference::fdT(double S)
{
    return PtrFDM->fT(S);
}

double FiniteDifference::fdL_dCdS()
{
    return PtrFDM->fL_dCdS();
}

double FiniteDifference::fdU_dCdS()
{
    return PtrFDM->fU_dCdS();
}

double FiniteDifference::getDelta()
{
    return (C[0][M+1]-C[0][M-1])/(S[0][M+1]-S[0][M-1]);
}

double FiniteDifference::getGamma()
{
    return ( (C[0][M+1]-C[0][M])/(S[0][M+1]-S[0][M])-(C[0][M]-C[0][M-1])/(S[0][M]-S[0][M-1]) )/(0.5*(S[0][M+1]-S[0][M-1]));
}

double FiniteDifference::getTheta()
{
    return ( C[1][M]-C[0][M] )/delta_t;
}


