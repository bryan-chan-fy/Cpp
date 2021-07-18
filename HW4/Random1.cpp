#include "Random1.h"
#include <cstdlib>
#include <cmath>

#include <iostream>

#define pi 4.0*atan(1.0)

const long a=16807;
const long m=2147483647;
const long q=127773;
const long r=2836;

using std::cout;
using std::endl;

double RandomBase::GetGaussian()
{
    double U1;
    double U2;

    U1 = GetUniform();
    U2 = GetUniform();

    return sqrt(-2.0*log(U1))*cos(2.0*pi*U2);       // BoxMuller Gaussian
}

ParkMiller::ParkMiller(long Seed_): Seed(Seed_)
{
    if (Seed==0)
        Seed=1;
}

void ParkMiller::SetSeed(long Seed_)
{
    Seed=Seed_;
    if (Seed==0)
        Seed=1;
}

long ParkMiller::GetOneRandomInteger()
{
    long k;
    k=Seed/q;
    Seed=a*(Seed-k*q)-r*k;

    if (Seed<0)
        Seed +=m;

    return Seed;
}

long ParkMiller::Max()
{
    return m-1;
}

RandomParkMiller::RandomParkMiller(long Seed_): ParkMillerGenerator(Seed_),InitialSeed(Seed_)
{
}

RandomBase* RandomParkMiller::clone() const
{
    return new RandomParkMiller(*this);
}

void RandomParkMiller::SetSeed(long Seed_)
{
    InitialSeed = Seed_;
    ParkMillerGenerator.SetSeed(Seed_);
}

double RandomParkMiller::GetUniform()
{
    return ParkMillerGenerator.GetOneRandomInteger()/(1.0+ParkMillerGenerator.Max());  // (rand()+1.0)/(RAND_MAX+1.0);
}



