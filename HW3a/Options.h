#ifndef OPTION_H
#define OPTION_H

#include "BSModel.h"

class Option
{
public:
    double T,K;
    bool isAmerican;    // is American or European option
    virtual double PayOff(double S_)=0;
    virtual double UpperBound_dCdS()=0;
    virtual double LowerBound_dCdS()=0;

};

class Call: public Option
{
public:
    double K;
    Call (double K_,double T_,bool isAmerican_) {K=K_; T=T_; isAmerican=isAmerican_;}
    double PayOff(double S);    // Option PayOff
    double UpperBound_dCdS();   // Upper Boundary dC/dS
    double LowerBound_dCdS();   // Lower Boundary dC/dS
};

class Put: public Option
{
public:
    double K;
    Put (double K_,double T_,bool isAmerican_) {K=K_; T=T_; isAmerican=isAmerican_;}
    double PayOff(double S);    // Option PayOff
    double UpperBound_dCdS();   // Upper Boundary dC/dS
    double LowerBound_dCdS();   // Lower Boundary dC/dS
};

#endif
