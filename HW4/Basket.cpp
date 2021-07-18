#include "Basket.h"
#include "Random1.h"
#include "Cholesky.h"
#include "writecsv.h"
#include "PayOff.h"
#include <vector>
#include <cmath>
#include <iostream>

RandomParkMiller RNDGenerator4(100);

using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::string;

typedef double myarray[3][3];


void BM_Asset::GenerateAssets(int ndays,int num_asset,double X0[3],double mu[3],double sigm[3],int M, double CorrMat[][3],mymatrix& X)
{
    double dt=1.0/365;
    double RND[num_asset];
    double Xt[num_asset];

    myarray LowerMat;
    double sum_rndvar=0.0;
    double asset_price=0.0;

    typedef vector<pair<string,vector<double>>> VectorPair;

    VectorPair OutputDataset;		// For saving data to CSV
    vector<double> tVec;         // storing array values for writing CSV
    vector<double> kVec, x1Vec, x2Vec, x3Vec, x4Vec, x5Vec;

    Cholesky_decompose(CorrMat,LowerMat);

    for (int j=0;j<M;j++)
    {
        for (int kk=0;kk<num_asset;kk++)     // Initial value
        {   Xt[kk]=X0[kk];
        }

        for (int i=0;i<ndays;i++)
        {
                for (int kk=0;kk<num_asset;kk++)            // Store random number
                {   RND[kk]=RNDGenerator4.GetGaussian();
                    }

                for (int k=0; k<num_asset; k++)     // Generate path for kth asset
                {
                    sum_rndvar=0.0;
                    for (int g=0;g<=k;g++)                  // Independent dW1,dW2,dW3
                            sum_rndvar+=RND[g]*LowerMat[k][g];
                    X[i][j][k]=Xt[k] + mu[k]*Xt[k]*dt + sigm[k]*Xt[k]*sqrt(dt)*sum_rndvar;
                    Xt[k]=X[i][j][k];
                }
        }
    }

    // preparation of data for CSV
    for (int k=0;k<num_asset;k++)
    {
        for (int i=0;i<ndays;i++)
        {
            tVec.push_back(i);
            kVec.push_back(k);              // kth asset
            x1Vec.push_back(X[i][0][k]);    // 5 different paths
            x2Vec.push_back(X[i][1][k]);
            x3Vec.push_back(X[i][2][k]);
            x4Vec.push_back(X[i][3][k]);
            x5Vec.push_back(X[i][4][k]);
        }
    }

    // writing to CSV file for plotting
    OutputDataset = {{"time",tVec},{"kAsset",kVec},{"Xpath1",x1Vec},{"Xpath2",x2Vec},{"Xpath3",x3Vec},{"Xpath4",x4Vec},{"Xpath5",x5Vec}};
    write_csv("MonteCarloData.csv",OutputDataset);       // write data to CSV file for R plotting

};


double BM_Asset::PriceAssetVanilla(int ndays, int num_asset, double dt, int M, mymatrix& X, double a[3],double r, double K, bool isCall)
{
    double Ut=0;
    double PayOff_T=0.0;
    double T=ndays*dt;
    double sum_CT=0;
    double sum_CT2=0;
    double SD;
    double SE;

    for (int j=0;j<M;j++)
    {
        Ut=0.0;
        for (int k=0; k<num_asset; k++)                // Compute U(t)
            Ut += a[k]*X[ndays-1][j][k];
        if (isCall==true) {
            PayOff_T=PayOff::GetInstance().CallPayOff(Ut,K);
        } else {
            PayOff_T=PayOff::GetInstance().PutPayOff(Ut,K);}

        sum_CT=sum_CT+PayOff_T;
        sum_CT2=sum_CT2+PayOff_T*PayOff_T;
    }
    SD=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*r*T)/(M-1));      // standard deviation
    SE=SD/sqrt(M);                                              // standard error
    cout << " SD SE " << SD << " " << SE <<endl;
    return exp(-r*T)*(1.0/M)*sum_CT;

}

double BM_Asset::PriceAssetExotic(int ndays, int num_asset, double dt, int M, mymatrix& X, double a[3],double r, double K)
{
    double Ut=0;
    double B=104.0;
    bool hitB=false;
    bool X2_GT_X3=false;
    double PayOff_T=0.0;
    double T=ndays*dt;
    double sum_CT=0;
    double sum_CT2=0;
    double SD;
    double SE;
    double X2max, X3max;
    double X2avg, X3avg;

    for (int j=0;j<M;j++)
    {
        X2max=0.0;
        X3max=0.0;
        for (int i=0;i<ndays;i++)
        {
            X2avg+=(1/M)*X[i][j][1];
            X3avg+=(1/M)*X[i][j][2];
            if (X[i][j][1]>X2max)   // Find asset 2 maximum
                X2max=X[i][j][1];
            if (X[i][j][2]>X3max)   // Find asset 3 maximum
                X3max=X[i][j][2];
            if (X[i][j][1]>=B)     // 2nd asset crosses Barrier
                hitB=true;
            if (X2max>X3max)        // 2nd asset max > 3rd asset max
                X2_GT_X3=true;
        }

        Ut=0.0;
        for (int k=0; k<num_asset; k++)         // Compute default U(t)
            Ut += a[k]*X[ndays-1][j][k];

        if (hitB==true)             // Define payout according to different conditions
            PayOff_T=PayOff::GetInstance().CallPayOff(X[ndays-1][j][1],K);
        else if (X2_GT_X3==true)
            PayOff_T=PayOff::GetInstance().CallPayOff(X[ndays-1][j][1]*X[ndays-1][j][1],K);
        else if (X2avg>X3avg)
            PayOff_T=PayOff::GetInstance().CallPayOff(X2avg,K);
        else PayOff_T=PayOff::GetInstance().CallPayOff(Ut,K);

        sum_CT=sum_CT+PayOff_T;
        sum_CT2=sum_CT2+PayOff_T*PayOff_T;
    }

    SD=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*r*T)/(M-1));       // standard deviation
    SE=SD/sqrt(M);                                              // standard error
    cout << " SD SE " << SD << " " << SE<<endl;
    return exp(-r*T)*(1.0/M)*sum_CT;

}

