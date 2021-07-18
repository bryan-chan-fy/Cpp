#include "VAR.h"
#include "Random1.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

RandomParkMiller RNDGenerator1(100);
RandomParkMiller RNDGenerator2(1000);
RandomParkMiller RNDGenerator3(5000);

using std::vector;
using std::cout;
using std::endl;
using std::sort;

typedef vector<double> SamplePath;

void PortfolioAsset::GenerateAssetStock(int ndays,double X0,int M)
{
    double dt=1.0/(252*4);     // 0.001
    double Xt = X0;
    double sum_RT=0;
    double sum_RT2=0;
    double SD=0;
    int n=0;
    double sum_n=0;
    int var_ind;
    SamplePath X(ndays*4);
    SamplePath ret_X(M);
    for (int j=0;j<M;j++)            // Generate Path for Asset
    {
        Xt = X0;
        for (int i=0; i<4*ndays; i++)
        {
            X[i]=Xt + 0.01*Xt*dt + 0.3*Xt*sqrt(dt)*RNDGenerator1.GetGaussian();
            Xt=X[i];
        }
        ret_X[j]=log(X[4*ndays-1]/X0);  // Compute Log Return
        sum_RT=sum_RT+ret_X[j];
        sum_RT2=sum_RT2+ret_X[j]*ret_X[j];
    }
    SD=sqrt((sum_RT2-sum_RT*sum_RT/M)/(M-1));
    cout << "VAR at 0.01 assuming Normal is " << sum_RT/M-2.326348*SD << endl;

    sort(ret_X.begin(),ret_X.end());    // sort the returns
    var_ind=ceil(0.01*M);               // index for lower 1% percentile
    cout << "VAR at 0.01 is " << ret_X[var_ind] << endl;

    for (int j=0;j<M;j++)               // compute CVAR
    {
        if (ret_X[j]<=ret_X[var_ind]) {
            n +=1;
            sum_n=sum_n + ret_X[j];
        }
    }
    cout << "CVAR at 0.01 is " << sum_n/n << endl;
}

void PortfolioAsset::GenerateAssetTBill(int ndays,double Y0,int M)
{
    double dt=1.0/(252*4);     // 0.001
    double Yt = Y0;
    double sum_RT=0;
    double sum_RT2=0;
    double SD=0;
    int n=0;
    double sum_n=0;
    int var_ind;
    SamplePath Y(ndays*4);
    SamplePath ret_Y(M);
    for (int j=0;j<M;j++)               // Generate Path for Asset
    {
        Yt = Y0;
        for (int i=0; i<4*ndays; i++)
        {
            Y[i]=Yt + 100*(90000+1000*i*dt-Yt)*dt + sqrt(Yt)*sqrt(dt)*RNDGenerator2.GetGaussian();
            Yt=Y[i];
        }
        ret_Y[j]=log(Y[4*ndays-1]/Y0);      // Compute Log Return
        //cout << ret_Y[j] << endl;
        sum_RT=sum_RT+ret_Y[j];
        sum_RT2=sum_RT2+ret_Y[j]*ret_Y[j];
    }
    SD=sqrt((sum_RT2-sum_RT*sum_RT/M)/(M-1));
    cout << "VAR at 0.01 assuming Normal is " << sum_RT/M-2.326348*SD << endl;

    sort(ret_Y.begin(),ret_Y.end());        // sort the returns
    var_ind=ceil(0.01*M);                   // index for lower 1% percentile
    cout << "VAR at 0.01 is " << ret_Y[var_ind] << endl;

    for (int j=0;j<M;j++)                   // compute CVAR
    {
        if (ret_Y[j]<=ret_Y[var_ind]) {
            n +=1;
            sum_n=sum_n + ret_Y[j];
        }
    }
    cout << "CVAR at 0.01 is " << sum_n/n << endl;
}

void PortfolioAsset::GenerateAssetFX(int ndays,double Z0,int M)
{
    double dt=1.0/(252*4);     // 0.001
    double Zt = Z0;
    double sum_RT=0;
    double sum_RT2=0;
    double SD=0;
    int n=0;
    double sum_n=0;
    int var_ind;
    SamplePath Z(ndays*4);
    SamplePath ret_Z(M);
    for (int j=0;j<M;j++)                   // Generate Path for Asset
    {
        Zt = Z0;
        for (int i=0; i<4*ndays; i++)
        {
            Z[i]=Zt + 5*(6-Zt)*dt + 0.01*sqrt(Zt)*sqrt(dt)*RNDGenerator3.GetGaussian();
            Zt=Z[i];
        }
        ret_Z[j]=log(Z[4*ndays-1]/Z0);      // Compute Log Return
        //cout << ret_X[j] << endl;
        sum_RT=sum_RT+ret_Z[j];
        sum_RT2=sum_RT2+ret_Z[j]*ret_Z[j];
    }
    SD=sqrt((sum_RT2-sum_RT*sum_RT/M)/(M-1));
    cout << "VAR at 0.01 assuming Normal is " << sum_RT/M-2.326348*SD << endl;

    sort(ret_Z.begin(),ret_Z.end());        // sort the returns
    var_ind=ceil(0.01*M);                   // index for lower 1% percentile
    cout << "VAR at 0.01 is " << ret_Z[var_ind] << endl;

    for (int j=0;j<M;j++)                   // compute CVAR
    {
        if (ret_Z[j]<=ret_Z[var_ind]) {
            n +=1;
            sum_n=sum_n + ret_Z[j];
        }
    }
    cout << "CVAR at 0.01 is " << sum_n/n << endl;
}
