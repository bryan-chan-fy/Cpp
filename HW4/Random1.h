#ifndef Random1_h
#define Random1_h

class RandomBase
{
public:
    RandomBase() {};
    virtual RandomBase* clone() const=0;
    virtual void SetSeed(long Seed)=0;
    virtual double GetUniform()=0;

    virtual double GetGaussian();       // non-pure virtual
    ~RandomBase() {};
private:
};

class ParkMiller                        // A class which adapts its interface
{
public:
    ParkMiller(long Seed=1);
    long GetOneRandomInteger();
    void SetSeed(long Seed);

    static long Max();

private:
    long Seed;
};

class RandomParkMiller : public RandomBase
{
public:
    RandomParkMiller(long Seed);
    virtual RandomBase* clone() const;
    virtual void SetSeed(long Seed);
    virtual double GetUniform();
    ~RandomParkMiller(){};
private:
    ParkMiller ParkMillerGenerator;         // adapter Design Pattern
    long InitialSeed;
};

#endif
