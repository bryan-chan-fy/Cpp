#include "Functions.h"
#include "Numeric.h"
#include "BSModel.h"

#include <iostream>
#include <complex>
#include <cmath>

using namespace std;

int main()
{
    SampleFunction1 f;

    const double pi=std::acos(-1);
    const std::complex<double> i(0,1);

    std::complex<double> psi_v;
    std::complex<double> sum_FFT;
    std::complex<double> CallPrice;

    int n_int=4000;
    double r=0.06;
    double v=0.1;
    double alpha=1.7;
    double T=1;
    double Sigma=0.2;
    double Spot=100; //50
    double t1=-250;
    double t2=250;
    double delta=0.02; //0.03

    double b=20;
    int N=200;
    double lambda=2*b/N;
    double eta=(2*pi/N)/lambda;
    int u;
    double k_u;

    NumericImplement CharacFunc(t1,t2,n_int,f,Sigma,Spot,r,delta,T,v,alpha);

    u=123;
    sum_FFT=0.0;
    k_u=-b+lambda*(u-1);

    cout << " k_u " << k_u << endl;

    // FFT using summation to calculate CT(k)
    for (int j=1;j<=N;j++)
    {
        v=(j-1)*eta;
        //cout << v << " v " << endl;

        NumericImplement CharacFunc(t1,t2,n_int,f,Sigma,Spot,r,delta,T,v,alpha);

        psi_v=exp(-r*T)*CharacFunc.Integral_Trapz()/(alpha*alpha+alpha-v*v+(2*alpha+1)*v*i);

        sum_FFT += exp(-i*(2*pi/N)*(double)(1.0*(j-1))*(double)(1.0*(u-1)))*exp(i*b*v)*psi_v*(eta/3.0)*(3.0+pow(-1.0,j)-delta_func(j));
    }

    CallPrice=exp(-alpha*k_u)/pi*sum_FFT;

    cout << CallPrice.real() << " is the Call Price from FFT with Strike " << exp(k_u) << endl;

    BSModel S_Model(Spot,r,delta,Sigma);
    cout << " Theoretical BS Model Call Price is " << endl;
    S_Model.GetOptionPrice(T,exp(k_u));

    return 0;
}
