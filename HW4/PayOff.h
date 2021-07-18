#ifndef PayOff_h
#define PayOff_h


class PayOff {                      // Singleton design pattern
public:
    static PayOff& GetInstance();
    //static PayOff* GetInstance();
    void ReserveFunction();
    double CallPayOff(double Spot, double Strike);
    double PutPayOff(double Spot,double Strike);

private:                            // these are all private because of Singleton
    PayOff();
    ~PayOff();

    PayOff(const PayOff& original); // copy constructor should not be implementable
    PayOff& operator=(const PayOff& original); // Singleton should not be assignable

    static PayOff PayOff_;          // need to declare
    //static PayOff* PayOff_;
};

#endif
