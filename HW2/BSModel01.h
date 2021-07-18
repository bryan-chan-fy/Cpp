#ifndef BSModel1_h
#define BSModel1_h

// General Option Class
class BS_Option
{
public:
    BS_Option(double Spot_,double Strike_, double r_, double q_, double Expiry_);
    virtual double operator()(double Sigma_) const=0;
    virtual double GetDelta() {};
    //virtual double GetGamma(double Sigma_) const=0;
    //virtual double GetVega(double Sigma_) const=0;
    ~BS_Option() {}
protected:
    double Spot;
    double Strike;
    double r;
    double q;
    double Expiry;
private:
};

// BS_Call inherits BS_Option
class BS_Call: public BS_Option
{
public:
    BS_Call(double Spot_,double Strike_, double r_, double q_, double Expiry_);
    double operator()(double Sigma_) const;  // return Call Price with Sigma input
    double GetDelta(double Sigma_) const;
    double GetGamma(double Sigma_) const;
    double GetVega(double Sigma_) const;
    ~BS_Call(){};

};

// BS_Put inherits BS_Option
class BS_Put: public BS_Option
{
public:
    BS_Put(double Spot_,double Strike_, double r_, double q_ ,double Expiry_);
    double operator()(double Sigma_) const;  // return Put Price with Sigma input
    ~BS_Put(){};

};

double normalCDF(double x);

double normalPDF(double x);

#endif
