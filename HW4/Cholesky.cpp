#include "Cholesky.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

using std::cout;
using std::endl;
using std::setw;

typedef double myarray[3][3];

void Cholesky_decompose(double A[][3],myarray& L)
{
    int n=sizeof(L)/sizeof(L[0]);
    // Initial array with zeros
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            L[i][j]=0;

    for (int i=0;i<n;i++)
    {
        for (int j=0;j<=i;j++)
        {
        double sum=0;

        if (j==i)  // compute diagonals L(j,j)
        {
            for (int k=0;k<j;k++)
                sum += pow(L[j][k],2);
            L[j][j]=sqrt(A[j][j]-sum);
        } else {
            // compute L(i,j) using L(j,j)
            for (int k=0;k<j;k++)
                sum += L[i][k]*L[j][k];
            L[i][j]=1/L[j][j]*(A[i][j]-sum);
        }

        }
    }

    // Display L
    cout << setw(6) << " L= " << endl;
    for (int i=0;i<n;i++) {
        // L
        for (int j=0;j<n;j++)
            cout << setw(6) << L[i][j] << "\t";
        cout << endl;
    }
}
