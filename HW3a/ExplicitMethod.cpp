#include "ExplicitMethod.h"

#define max(a,b) a>b? a:b

#include <iostream>

using std::cout;
using std::endl;

double ExplicitMethod::SolvePDE()
{
    // compute payoff at maturity
    for (int j=0;j<=2*M; j++)
    {
        C[N][j]= fdT(S[N][j]);
    }

    // step back through lattice
    for (int i=N-1;i>=0;i--)
    {
        for (int j=1;j<=2*M-1;j++)
        {
            C[i][j] = pu*C[i+1][j+1]+pm*C[i+1][j]+pd*C[i+1][j-1];
        }

        // boundary conditions for high and low asset prices
        C[i][2*M]=C[i][2*M-1]+fdU_dCdS()*(S[i][2*M]-S[i][2*M-1]);
        C[i][0]=C[i][1]-fdL_dCdS()*(S[i][1]-S[i][0]);

        // extra steps if American
        if (isAmerican==true)
        {
            for (int j=0;j<=2*M;j++)
            {
                C[i][j]=max(fdT(S[i][j]),C[i][j]);
            }
        }

    }
    return C[0][M];
}
