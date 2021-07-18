#ifndef Option_h

#include "BSModel.h"

class Option
{
public:
    double T;
    double K;
    int N;
    bool isCall;
    double SD_array[2];				// For standard deviation & standard error
    double PriceMC(BSModel Model,int M);
    double PriceMCAntVar(BSModel Model,int M);
    double PriceMCDeltaCV(BSModel Model,int M);
    double PriceMCAntCVCombined(BSModel Model,int M);
    virtual double GetPayOff(SamplePath& S)=0;
};

class EuropeanOption : public Option
{
public:
    EuropeanOption (double T_,double K_,int N_,bool isCall_) {T=T_;K=K_;N=N_;isCall=isCall_;}
    virtual double GetPayOff(SamplePath& S);
};


#endif // Option_h
