#ifndef Options_h

#include "Heston.h"

class Option
{
public:
    double T;
    double K;
    int N;
    bool isCall;
    double SD_array[2];				// For standard deviation & standard error
    double PriceMC(HSModel Model,int M, int method);
    double PriceMC_Milhstein(HSModel Model,int M, int method);
    double PriceMLMC(HSModel,int M);
    double PriceMC_Debias(HSModel Model,int M);
    virtual double GetPayOff(double ST)=0;
};

class EuropeanOption : public Option
{
public:
    EuropeanOption (double T_,double K_,int N_,bool isCall_) {T=T_;K=K_;N=N_;isCall=isCall_;}
    virtual double GetPayOff(double ST);
};


#endif // Options_h
