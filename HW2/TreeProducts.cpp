#include "TreeProducts.h"
#include "Options.h"
#include "myenum.h"

#define max(a,b) a>b?a:b

using namespace myenum;

TreeAmerican::TreeAmerican(double Strike_, OptionType OptionType_):Strike(Strike_),TheOptionType(OptionType_)
{
}

// Final PayOff
double TreeAmerican::FinalPayOff(double Spot) const
{
    PayOff ThisPayOff(Strike,TheOptionType);
    return ThisPayOff(Spot);
};

double TreeAmerican::SelectPayOff(double Spot,double ContinueVal) const
{
    PayOff ThisPayOff(Strike,TheOptionType);
    return max(ThisPayOff(Spot),ContinueVal);    // American Option PayOff choose max(Exercise payoff,Cont. Val)
}

TreeProduct* TreeAmerican::clone() const
{
    return new TreeAmerican(*this);
}

TreeEuropean::TreeEuropean(double Strike_,OptionType OptionType_):Strike(Strike_),TheOptionType(OptionType_)
{
}

// Final PayOff
double TreeEuropean::FinalPayOff(double Spot) const
{
    PayOff ThisPayOff(Strike,TheOptionType);
    return ThisPayOff(Spot);
}

double TreeEuropean::SelectPayOff(double Spot,double ContinueVal) const
{
    return ContinueVal;    // European Option PayOff
}

TreeProduct* TreeEuropean::clone() const
{
    return new TreeEuropean(*this);
}
