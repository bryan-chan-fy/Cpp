#include "PayOff.h"

#define max(a,b) a>b? a:b

PayOff::PayOff()    // private constructor, cannot be used by clients
{
}

PayOff::~PayOff()   // this is private and cannot be used by clients
{
}

void PayOff::ReserveFunction()
{
    //
}

double PayOff::CallPayOff(double Spot, double Strike)
{
    return max(Spot-Strike,0);
}

double PayOff::PutPayOff(double Spot,double Strike)
{
    return max(Strike-Spot,0);
}

PayOff PayOff::PayOff_;                      //  allocate & initialize static data member.
//PayOff* PayOff::PayOff_ = nullptr;         //  pointer being allocated


//PayOff* PayOff::GetInstance()
PayOff& PayOff::GetInstance()
{
    //if (!PayOff_) {                        // for pointers only
    //    PayOff_ = new PayOff(); }
    return PayOff_;
}
