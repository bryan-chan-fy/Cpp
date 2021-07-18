#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Just an interface
class MathFunction
{
public:
    virtual double operator()(double x)=0;
    virtual double operator()(double x, double y)=0;
    virtual ~MathFunction() {}
private:
};

class SampleFunction1: public MathFunction
{
public:
    virtual double operator()(double x);
    virtual double operator()(double x,double y) {};
    virtual ~SampleFunction1();
};

class SampleFunction2D1: public MathFunction
{
public:
    virtual double operator()(double x){};
    virtual double operator()(double x,double y);
    virtual ~SampleFunction2D1();
};

class SampleFunction2D2: public MathFunction
{
public:
    virtual double operator()(double x){};
    virtual double operator()(double x,double y);
    virtual ~SampleFunction2D2();
};

#endif
