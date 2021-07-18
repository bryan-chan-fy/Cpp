#include "CrankNicolson.h"

#define max(a,b) a>b? a:b

#include <iostream>

using std::cout;
using std::endl;

typedef vector<double> Vector;

double CrankNicolson::SolvePDE()
{
    // compute payoff at maturity
    for (int j=0;j<=2*M; j++)
    {
        C[N][j]= fdT(S[N][j]);
        // cout << C[N][j] << endl;
    }

    // step back through lattice
    for (int i=N-1;i>=0;i--)
    {
        SolveTridiagonal(i);            // Solve Tridiagonal matrix equations

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


void CrankNicolson::SolveTridiagonal(int index_)
{
    // Solves the Tridiagonal Matrix equations iteratively from bottom to top, then top down
    Vector pmp,pp;
    int i=index_;

    pmp.resize(2*M+1);
    pp.resize(2*M+1);

    // compute boundary conditions
    double lambda_U= fdU_dCdS()*(S[i][2*M]-S[i][2*M-1]);
    double lambda_L= fdL_dCdS()*(S[i][1]-S[i][0]);

    // setup the Q array
    Q[i+1][2*M]=lambda_U;
    Q[i+1][0]=lambda_L;
    for (int j=1;j<=2*M-1;j++)
    {
        Q[i+1][j]=-pu*C[i+1][j+1]-(pm-2)*C[i+1][j]-pd*C[i+1][j-1];
    }

    // lower boundary
    pmp[1]=pm+pd;
    pp[1]=Q[i+1][1] + pd*Q[i+1][0];

    for (int j=2;j<=2*M-1;j++)
    {
        pmp[j]=pm- pu*pd/pmp[j-1];
        pp[j]=Q[i+1][j]-pp[j-1]*pd/pmp[j-1];
    }

    // upper boundary
    C[i][2*M]=(pp[2*M-1]+pmp[2*M-1]*lambda_U)/(pu+pmp[2*M-1]);
    C[i][2*M-1]=C[i][2*M]-lambda_U;

    // back substitution
    for (int j=2*M-2;j>=1;j--)
    {
        C[i][j]=(pp[j]-pu*C[i][j+1])/pmp[j];
    }

    C[i][0]=C[i][1]-fdL_dCdS()*(S[i][1]-S[i][0]);

}
