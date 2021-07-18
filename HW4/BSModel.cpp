#include "BSModel.h"
#include "Random1.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>

using std::vector;
using std::cout;
using std::endl;

RandomParkMiller RNDGenerator(100);

const double pi=4.0*atan(1.0);

BSModel::BSModel(double S0_,double r_,double delta_,double Vol_):S0(S0_),r(r_), delta(delta_), Vol(Vol_)
{
}

void BSModel::GenerateSamplePath(double T,int N, SamplePath& S)     // Generate stock path
{
    double dt=T/N;
    double nu=(r-delta-0.5*Vol*Vol);
    double St = S0;
    for (int i=0; i<N; i++)
    {
        S[i]=St*exp(nu*dt+Vol*sqrt(dt)*RNDGenerator.GetGaussian());
        St=S[i];
    }
}

void BSModel::GenerateSamplePath(double T,int N, SamplePath& S1,SamplePath& S2)  // 2 stock path for Antithetic
{
    double dt=T/N;
    double nu=(r-delta-0.5*Vol*Vol);
    double RND;
    double St1 = S0;
    double St2 = S0;
    for (int i=0; i<N; i++)
    {
        RND=RNDGenerator.GetGaussian();
        S1[i]=St1*exp(nu*dt+Vol*sqrt(dt)*RND);      // Stock Path 1
        S2[i]=St2*exp(nu*dt+Vol*sqrt(dt)*(-RND));   // Stock Path 2
        St1=S1[i];
        St2=S2[i];
    }
}

double BSModel::GetDelta(double T,double K,double St)
{
    double d1;
    double d2;
    double Spot;

    if (St==0.0) {          // default Spot to S0 if St not specified else use St
        Spot=S0;
    } else { Spot=St;}

    d1 =(log(Spot/K) + (r-delta+Vol*Vol/2)*T)/(Vol*sqrt(T));
    d2 = d1 - Vol*sqrt(T);

    return exp(-delta*T)*normalCDF(d1);
}

void BSModel::GetOptionPrice(double T,double K)
{
    // Display Option Prices
    double d1;
    double d2;

    d1 =(log(S0/K) + (r-delta+Vol*Vol/2)*T)/(Vol*sqrt(T));
    d2 = d1 - Vol*sqrt(T);

    cout << " Call Price: " << S0*exp(-delta*T)*normalCDF(d1)-K*exp(-r*T)*normalCDF(d2) << endl;
    cout << " Put Price: " << K*exp(-r*T)*normalCDF(-d2)-S0*exp(-delta*T)*normalCDF(-d1) << endl;
}


double normalCDF (double x) {
    return 0.5* std::erfc(-x/sqrt(2));
}

double normalPDF(double x){
    return 1/sqrt(2*pi)*exp(-x*x/2);
}
