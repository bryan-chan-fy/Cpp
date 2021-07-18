#include "PutCallParity.h"
#include <cmath>

double PutCallParity(double OptionPrice, double Spot, double r, double Expiry, double Strike, bool isCall) {

// C + K exp(-r*T) = P + S

double OppositeOptionPrice;

    if (isCall){    // Get Call Price
        OppositeOptionPrice = OptionPrice + Spot - Strike*exp(-r*Expiry);
    } else {        // Get Put Price
        OppositeOptionPrice = OptionPrice - Spot + Strike*exp(-r*Expiry);
    }
    return OppositeOptionPrice;
}
