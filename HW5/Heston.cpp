#include "Heston.h"
#include "Random1.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>

using std::vector;
using std::cout;
using std::endl;

RandomParkMiller RNDGenerator1(100);
RandomParkMiller RNDGenerator2(30);

const double pi=4.0*atan(1.0);


double QuadratureIntegral(std::vector<double> V, double T, int N)
{
    double f_sum=0.0;
    double dt=T/N;

    for (int i=0;i<N;i++)
    {
        f_sum+=V[i];
    }
    return dt*f_sum;

}

HSModel::HSModel(double S0_, double V0_, double r_, double sigma_, double kappa_, double theta_, double rho_):S0(S0_),V0(V0_),r(r_), sigma(sigma_),kappa(kappa_), theta(theta_), rho(rho_)
{
}

double maxfunc(double a)
{
    if (a>0.0) {
        return a;
    } else {
        return 0.0;
    }
}

double absfunc(double a)
{
    return fabs(a);
}

double eqfunc(double a)
{
    return a;
}


void HSModel::GenerateSamplePath(double T,int N, SamplePath& S, SamplePath& V, int method)     // Generate stock path
{
    SamplePath VT(N);

    double (*f1)(double a);
    double (*f2)(double a);
    double (*f3)(double a);

    double dt=T/N;
    double St = S0;
    double Vt = V0;
    double VTt= Vt;

    double deltaWV;
    double deltaWS;

    for (int i=0; i<N; i++)
    {
        switch(method) {
        case 1:
            f1=&maxfunc; f2=&maxfunc; f3=&maxfunc; break;
        case 2:
            f1=&absfunc; f2=&absfunc; f3=&absfunc; break;
        case 3:
            f1=&eqfunc; f2=&eqfunc; f3=&absfunc; break;
        case 4:
            f1=&eqfunc; f2=&eqfunc; f3=&maxfunc; break;
        case 5:
            f1=&eqfunc; f2=&maxfunc; f3=&maxfunc; break;
        }
        deltaWV=sqrt(dt)*RNDGenerator1.GetGaussian();
        deltaWS=rho*deltaWV+sqrt(1-rho*rho)*sqrt(dt)*RNDGenerator2.GetGaussian();
        VT[i]=f1(VTt)-kappa*dt*(f2(VTt)-theta)+sigma*sqrt(f3(VTt))*deltaWV;
        V[i]=f3(VT[i]);
        S[i]=St*exp((r-0.5*Vt)*dt+sqrt(Vt)*deltaWS);
        St=S[i];
        Vt=V[i];
        VTt=VT[i];
    }
}

void HSModel::GenerateSamplePath_Milhstein(double T,int N, SamplePath& S, SamplePath& V, int method)     // Generate stock path
{
    SamplePath VT(N);

    double (*f1)(double a);
    double (*f2)(double a);
    double (*f3)(double a);

    double dt=T/N;
    double St = S0;
    double Vt = V0;
    double VTt= Vt;

    double deltaWV;
    double deltaWS;
    double Z;

    for (int i=0; i<N; i++)
    {
        switch(method) {
        case 1:
            f1=&maxfunc; f2=&maxfunc; f3=&maxfunc; break;
        case 2:
            f1=&absfunc; f2=&absfunc; f3=&absfunc; break;
        case 3:
            f1=&eqfunc; f2=&eqfunc; f3=&absfunc; break;
        case 4:
            f1=&eqfunc; f2=&eqfunc; f3=&maxfunc; break;
        case 5:
            f1=&eqfunc; f2=&maxfunc; f3=&maxfunc; break;
        }
        Z=RNDGenerator1.GetGaussian();
        deltaWV=sqrt(dt)*Z;
        deltaWS=rho*deltaWV+sqrt(1-rho*rho)*sqrt(dt)*RNDGenerator2.GetGaussian();
        VT[i]=f1(VTt)-kappa*dt*(f2(VTt)-theta)+sigma*sqrt(f3(VTt))*deltaWV+ 0.25*sigma*sigma*dt*(Z*Z-1);
        V[i]=f3(VT[i]);
        S[i]=St*exp((r-0.5*Vt)*dt+sqrt(Vt)*deltaWS);
        St=S[i];
        Vt=V[i];
        VTt=VT[i];
    }
}

void HSModel::GenerateSampleTerminalPairs(double T,int l, int M,VecArray& SVec)   // M=2
{

    int N=pow(M,l);
    SamplePath VT(N);
    SamplePath V(N);
    SamplePath S(N);

    int N_h=ceil(N/M);
    SamplePath VT_h(N_h);
    SamplePath V_h(N_h);
    SamplePath S_h(N_h);

    double (*f1)(double a);
    double (*f2)(double a);
    double (*f3)(double a);

    double dt=T/N;
    double St = S0;
    double Vt = V0;
    double VTt= Vt;

    double dt_h=M*dt;
    double St_h=S0;
    double Vt_h=V0;
    double VTt_h=Vt_h;

    double deltaWV;
    double deltaWS;

    double deltaWV_h=0;
    double deltaWS_h=0;


    int i_h=0;

    for (int i=0; i<N; i++)
    {
        f1=&maxfunc; f2=&maxfunc; f3=&maxfunc;

        deltaWV=sqrt(dt)*RNDGenerator1.GetGaussian();
        deltaWS=rho*deltaWV+sqrt(1-rho*rho)*sqrt(dt)*RNDGenerator2.GetGaussian();
        deltaWV_h+=deltaWV;
        deltaWS_h+=deltaWS;
        VT[i]=f1(VTt)-kappa*dt*(f2(VTt)-theta)+sigma*sqrt(f3(VTt))*deltaWV;
        V[i]=f3(VT[i]);
        S[i]=St*exp((r-0.5*Vt)*dt+sqrt(Vt)*deltaWS);
        if ((i+1)%M==0) {           // Handle the l-1 half number of sample path points
                VT_h[i_h]=f1(VTt_h)-kappa*dt_h*(f2(VTt_h)-theta)+sigma*sqrt(f3(VTt_h))*deltaWV_h;
                V_h[i_h]=f3(VT_h[i_h]);
                S_h[i_h]=St_h*exp((r-0.5*Vt_h)*dt_h+sqrt(Vt_h)*deltaWS_h);
                St_h=S_h[i_h];
                Vt_h=V_h[i_h];

                deltaWS_h=0;  // clear dWS_i+dWS_i+1
                deltaWV_h=0;
                i_h++;
        }
        St=S[i];
        Vt=V[i];
        VTt=VT[i];
    }
    SVec[1]=S[N-1];
    if (N>=M) { SVec[0]=S_h[N/M-1];} else {SVec[0]=0.0;}
}


void HSModel::GenerateEstimatorDebias(double T,int N, double K, double& Cn)     // Generate stock path
{
    SamplePath VT(N);
    SamplePath V(N);
    SamplePath S(N);

    double (*f1)(double a);
    double (*f2)(double a);
    double (*f3)(double a);

    double dt=T/N;
    double St = S0;
    double Vt = V0;
    double VTt= Vt;

    double deltaWV;
    double deltaWS;

    double MT=0.0;
    double HT=0.0;
    double aT=0.0;
    double bT=0.0;
    double exp_eq=0.0;
    double IT=0.0;

    double d1;
    double d2;

    for (int i=0; i<N; i++)
    {
        f1=&maxfunc; f2=&maxfunc; f3=&maxfunc;

        deltaWV=sqrt(dt)*RNDGenerator1.GetGaussian();
        deltaWS=rho*deltaWV+sqrt(1-rho*rho)*sqrt(dt)*RNDGenerator2.GetGaussian();
        VT[i]=f1(VTt)-kappa*dt*(f2(VTt)-theta)+sigma*sqrt(f3(VTt))*deltaWV;
        V[i]=f3(VT[i]);
        S[i]=St*exp((r-0.5*Vt)*dt+sqrt(Vt)*deltaWS);
        //cout << S[i] << endl;
        St=S[i];
        Vt=V[i];
        VTt=VT[i];
    }

    MT= QuadratureIntegral(V,T,N);
    IT=MT;
    HT= kappa*theta*T/sigma - kappa/sigma*IT;
    aT = (rho/sigma)*(V[N-1]-V0)-rho*HT-0.5*MT;
    bT = (1-rho*rho)*IT;
    d1 = (log(S0/K)+r*T+aT+bT)/sqrt(bT);
    d2 = d1 - sqrt(bT);
    Cn = S0*exp(aT+0.5*bT)*normalCDF(d1)-K*exp(-r*T)*normalCDF(d2);

}


double normalCDF (double x) {
    return 0.5* std::erfc(-x/sqrt(2));
}

double normalPDF(double x){
    return 1/sqrt(2*pi)*exp(-x*x/2);
}
