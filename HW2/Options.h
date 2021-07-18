#ifndef OPTIONS_H
#define OPTIONS_H

#include "myenum.h"

using namespace myenum;

// Handle Barrier Option Indicator & Theoretical Barrier Option Price
class BarrierOpt
{
public:
    BarrierOpt(){};
    BarrierOpt(double Spot_, double r_, double q_, double Sigma, double Expiry_);
    double GetUI_CallPrice(double Strike,double H);
    double GetUO_CallPrice(double Strike,double H);
    int UI_Indicator(double So, double H);
    int UO_Indicator(double So, double H);
    ~BarrierOpt(){};

private:
    double Spot;
    double r;
    double q;
    double Expiry;
    double Sigma;
    double nu=r-q-Sigma*Sigma/2;
};

class PayOff
{
public:

    // PayOff (const PayOff& original);
    PayOff(double Strike,OptionType TheOptionType);
    double operator() (double Spot) const;
    virtual ~PayOff(){}
    // virtual PayOff* clone() const=0;

private:
    double Strike;
    OptionType TheOptionType;

    //PayOff* ThePayOffPtr;
};

#endif
