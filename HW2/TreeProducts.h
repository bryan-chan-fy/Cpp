#ifndef TREEPRODUCTS_H
#define TREEPRODUCTS_H

#include "Options.h"
#include "myenum.h"

using namespace myenum;


class TreeProduct
{
public:
    TreeProduct(){};
    virtual double FinalPayOff(double Spot) const=0;
    virtual double SelectPayOff(double Spot,double ContinueVal) const=0;
    virtual ~TreeProduct(){}
    virtual TreeProduct* clone() const=0;

private:
};

class TreeAmerican: public TreeProduct
{
public:
    TreeAmerican(double Strike_, OptionType OptionType_);
    virtual TreeProduct* clone() const;
    virtual double FinalPayOff(double Spot) const;
    virtual double SelectPayOff(double Spot,double ContinueVal) const;
    virtual ~TreeAmerican(){}   // destructor


private:
    OptionType TheOptionType;
    double Strike;
};

class TreeEuropean: public TreeProduct
{
public:
    TreeEuropean(double Strike_, OptionType OptionType_);
    virtual TreeProduct* clone() const;
    virtual double FinalPayOff(double Spot) const;
    virtual double SelectPayOff(double Spot,double ContinueVal) const;
    virtual double SelectPayOffBarrier(double Spot,double ContinueVal,int Indicator) const;
    virtual ~TreeEuropean(){}   // destructor

private:
    OptionType TheOptionType;
    double Strike;

};

//double AmOption::PriceBySnell(...) // -> return (0,0) value

#endif
