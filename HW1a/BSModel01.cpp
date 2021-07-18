#include "BSModel01.h"
#include "Random1.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

#define pi 4.0*atan(1.0)

using std::sqrt;
using std::cout;
using std::endl;

// constructor
BS_Stock::BS_Stock(double Spot_,double r_, double Sigma_,double Expiry_):Spot(Spot_),r(r_),Sigma(Sigma_),Expiry(Expiry_)
{}

// method
double BS_Stock::GetStockPrice()
{
    double StockPrice;
    double Gaussian;

    Gaussian=GetBoxMullerGaussian();
    StockPrice = Spot*exp((r-Sigma*Sigma*0.5)*Expiry)*exp(Sigma*sqrt(Expiry)*Gaussian);
    return StockPrice;
}


BS_Option::BS_Option(double Spot_,double Strike_, double r_, double Expiry_):Spot(Spot_),Strike(Strike_),r(r_),Expiry(Expiry_)
{}

// return Call Price with Sigma input
double BS_Call::operator()(double Sigma_) const
{
    double d1;
    double d2;
    double CallPrice;

    d1 =(log(Spot/Strike) + (r+Sigma_*Sigma_/2)*Expiry)/(Sigma_*sqrt(Expiry));
    d2 = d1 - Sigma_*sqrt(Expiry);

    CallPrice = Spot*normalCDF(d1)-Strike*exp(-r*Expiry)*normalCDF(d2);

    return CallPrice;
}

// return delta of Call Option (q=0, no dividend)
double BS_Call::GetDelta(double Sigma_) const
{
    double d1;

    d1 =(log(Spot/Strike) + (r+Sigma_*Sigma_/2)*Expiry)/(Sigma_*sqrt(Expiry));
    return normalCDF(d1);
}

// return Gamma of Call Option (q=0, no dividend)
double BS_Call::GetGamma(double Sigma_) const
{
    double d1;

    d1 =(log(Spot/Strike) + (r+Sigma_*Sigma_/2)*Expiry)/(Sigma_*sqrt(Expiry));

    return normalPDF(d1)/(Strike*Sigma_*sqrt(Expiry));
}

// return Vega of Call Option (q=0, no dividend)
double BS_Call::GetVega(double Sigma_) const
{
    double d1;

    d1 =(log(Spot/Strike) + (r+Sigma_*Sigma_/2)*Expiry)/(Sigma_*sqrt(Expiry));

    return Strike*normalPDF(d1)*sqrt(Expiry);
}


// BS_Call constructor
BS_Call::BS_Call(double Spot_,double Strike_, double r_, double Expiry_):BS_Option(Spot_,Strike_,r_,Expiry_)
{}

// return Put Price with Sigma input
double BS_Put::operator()(double Sigma_) const
{
    double d1;
    double d2;
    double PutPrice;

    d1 =(log(Spot/Strike) + (r+Sigma_*Sigma_/2)*Expiry)/(Sigma_*sqrt(Expiry));
    d2 = d1 - Sigma_*sqrt(Expiry);

    PutPrice = Strike*exp(-r*Expiry)*normalCDF(-d2)-Spot*normalCDF(-d1);

    return PutPrice;
}

// BS_Put constructor
BS_Put::BS_Put(double Spot_,double Strike_, double r_, double Expiry_):BS_Option(Spot_,Strike_,r_,Expiry_)
{}

double normalCDF (double x) {
    return 0.5* std::erfc(-x/sqrt(2));
}

double normalPDF(double x){
    return 1/sqrt(2*pi)*exp(-x*x/2);
}
