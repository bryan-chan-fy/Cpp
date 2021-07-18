#include "Options.h"
#include "myenum.h"
#include "BSModel01.h"
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

#define max(a,b) a>b?a:b

PayOff::PayOff(double Strike_,OptionType TheOptionType_):Strike(Strike_),TheOptionType(TheOptionType_)
{
}

double PayOff::operator()(double Spot) const
{
    switch(TheOptionType)
    {
        case call:
            return max(Spot-Strike,0.0);
        case put:
            return max(Strike-Spot,0.0);
        default:
            throw("unknown option type");
    }
}

BarrierOpt::BarrierOpt(double Spot_, double r_, double q_, double Sigma_, double Expiry_): Spot(Spot_),r(r_),q(q_),Sigma(Sigma_),Expiry(Expiry_)
{}

double BarrierOpt::GetUI_CallPrice(double Strike,double H)
{
    double db1=(log(H/Spot) + (r-q+Sigma*Sigma/2)*Expiry)/(Sigma*sqrt(Expiry));
    double db2=(log(Spot/H) + (r-q+Sigma*Sigma/2)*Expiry)/(Sigma*sqrt(Expiry));

    BS_Put int_Put1(pow(H,2)/Spot,Strike,r,q,Expiry);   // Setup the Option parameters for internal Put 1
    BS_Put int_Put2(pow(H,2)/Spot,H,r,q,Expiry);        // Setup the Option parameters for internal Put 2
    BS_Call int_Call1(Spot,H,r,q,Expiry);               // Setup the Option parameters for internal Call 1

     return pow((H/Spot),2*nu/(Sigma*Sigma))*(int_Put1(Sigma)-int_Put2(Sigma)+(H-Strike)*exp(-r*Expiry)*normalCDF(-db1))+int_Call1(Sigma)+(H-Strike)*exp(-r*Expiry)*normalCDF(db2);

};

double BarrierOpt::GetUO_CallPrice(double Strike, double H)
{
    double db1=(log(Spot/H) + (r-q+Sigma*Sigma/2)*Expiry)/(Sigma*sqrt(Expiry));
    double db2=(log(H/Spot) + (r-q+Sigma*Sigma/2)*Expiry)/(Sigma*sqrt(Expiry));

    BS_Call int_Call1(Spot,Strike,r,q,Expiry);              // Setup the Option parameters for internal Call 1
    BS_Call int_Call2(Spot,H,r,q,Expiry);                   // Setup the Option parameters for internal Call 2
    BS_Call int_Call3(pow(H,2)/Spot,Strike,r,q,Expiry);     // Setup the Option parameters for internal Call 3
    BS_Call int_Call4(pow(H,2)/Spot,H,r,q,Expiry);          // Setup the Option parameters for internal Call 4

    return int_Call1(Sigma)-int_Call2(Sigma)-(H-Strike)*exp(-r*Expiry)*normalCDF(db1)- pow((H/Spot),2*nu/(Sigma*Sigma))*(int_Call3(Sigma)-int_Call4(Sigma)-(H-Strike)*exp(-r*Expiry)*normalCDF(db2));
};

int BarrierOpt::UI_Indicator (double So, double H)
{
        // for Up and In
        if (So>H) return 1;
            else return 0;
}

int BarrierOpt::UO_Indicator (double So, double H)
{
        // for Up and Out
        if (So>H) return 0;
            else return 1;
}
