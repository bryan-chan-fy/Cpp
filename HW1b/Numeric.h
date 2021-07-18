#ifndef NUMERIC_H
#define NUMERIC_H

#include "Functions.h"

//  Define classes
//  Numeric (Bridge) --> NumericInner (interface) --> NumericImplement
//  Bridging Design Pattern

// Create interface for Implementation
class NumericInner
{
public:
    NumericInner(){}
    virtual NumericInner* clone() const=0;   // virtual copy constructor
    virtual double Integral_Trapz() const=0;
    virtual double Integral_Simpson() const=0;
    virtual double Double_Integral() const=0;
    virtual ~NumericInner(){}
};


//  Bridge between outside and inner
class Numeric
{
public:
    Numeric(const NumericInner& inner_Object);   // this will be constructor in bridge design
    Numeric(const Numeric& original);
    Numeric& operator=(const Numeric& original); // assignment operator
    virtual ~Numeric();
    inline double Integral_Trapz() const;
    inline double Integral_Simpson() const;
    inline double Double_Integral()const;
private:
    NumericInner* InnerObjectPtr;  // Create a pointer for Inner Object
};

inline double Numeric::Integral_Trapz() const
{
    return InnerObjectPtr-> Integral_Trapz();
}

inline double Numeric::Integral_Simpson() const
{
    return InnerObjectPtr-> Integral_Simpson();
}

inline double Numeric::Double_Integral() const
{
    return InnerObjectPtr-> Double_Integral();
}

// Implementation
class NumericImplement : public NumericInner
{
public:
    NumericImplement(double t1, double t2, int n, MathFunction& f);
    NumericImplement(double t1, double t2, int n, double t3, double t4, int m, MathFunction& f); // For 2D integral
    virtual NumericInner* clone() const;
    virtual double Integral_Trapz() const;
    virtual double Integral_Simpson() const;
    virtual double Double_Integral() const;
private:
    double T1;
    double T2;
    double N;
    double T3;
    double T4;
    double M;
    MathFunction& F;
};

#endif
