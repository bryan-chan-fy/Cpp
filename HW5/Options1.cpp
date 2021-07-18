#include "Options.h"
#include "Random1.h"

RandomParkMiller RNDGenerator3(50);

#include <cmath>
#include <iostream>

#define max(a,b) a>b? a:b

using std::cout;
using std::endl;


int GetNlOptimum(int L, int M, double T, int l, std::vector<double> Vlvec, double epsilon)
{
    double sum_sqrt=0.0;
    double hl_,hl;

    for (int l_=0;l_<=L;l_++)
    {
        hl_=T/pow(M,l_);
        sum_sqrt+=sqrt(Vlvec[l_]/hl_);
    }
    hl=T/pow(M,l);
    return ceil((2.0/(epsilon*epsilon))*sqrt(Vlvec[l]*hl)*sum_sqrt);

}

double Option::PriceMC(HSModel Model,int M, int method)		// Simple MC
{
    double sum_CT=0.0;      // for Sum of PayOff
    double sum_CT2=0.0;     // for Sum of PayOff^2
    SamplePath S(N);
    SamplePath V(N);

    for (int j=0;j<M;j++)
    {
        Model.GenerateSamplePath(T,N,S,V,method);
        sum_CT=sum_CT+GetPayOff(S[N-1]);
        sum_CT2=sum_CT2+GetPayOff(S[N-1])*GetPayOff(S[N-1]);
    }
    cout << " sum CT CT2 " << sum_CT << " " << sum_CT2 << endl;
    SD_array[0]=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*Model.r*T)/(M-1));
    SD_array[1]=SD_array[0]/sqrt(M);
    return exp(-Model.r*T)*(1.0/M)*sum_CT;
}

double Option::PriceMC_Milhstein(HSModel Model,int M, int method)		// Simple MC
{
    double sum_CT=0.0;      // for Sum of PayOff
    double sum_CT2=0.0;     // for Sum of PayOff^2
    SamplePath S(N);
    SamplePath V(N);

    for (int j=0;j<M;j++)
    {
        Model.GenerateSamplePath_Milhstein(T,N,S,V,method);
        sum_CT=sum_CT+GetPayOff(S[N-1]);
        sum_CT2=sum_CT2+GetPayOff(S[N-1])*GetPayOff(S[N-1]);
    }
    cout << " sum CT CT2 " << sum_CT << " " << sum_CT2 << endl;
    SD_array[0]=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*Model.r*T)/(M-1));
    SD_array[1]=SD_array[0]/sqrt(M);
    return exp(-Model.r*T)*(1.0/M)*sum_CT;
}

double Option::PriceMLMC(HSModel Model,int M)
{
    VecArray SVec(2);

    int NL=10000;
    int L=0;
    double VL;
    double epsilon=0.01;
    double PL,PL_1;
    double sum_YL=0.0;
    double Y_est=0.0;
    double samples;
    double samples_sum=0.0;
    double samples_sq_sum=0.0;
    std::vector<double> Vlvec(100);
    std::vector<int> Nlopt(100);
    std::vector<double> YL(100);
    bool Converged=false;

    while ((L<2)|| (Converged==false))
    {
        cout << L << endl;
        for (int l_=L;l_<=L;l_++)
        {
            cout << "l_ running at " << l_ <<endl;
            samples_sum=0.0;
            samples_sq_sum=0.0;
            for (int i=0;i<NL;i++)
            {
                Model.GenerateSampleTerminalPairs(T,l_,2,SVec);
                PL=GetPayOff(SVec[1]);
                if (l_>=1) { PL_1=GetPayOff(SVec[0]);} else {PL_1=0;}
                samples=exp(-Model.r*T)*(PL-PL_1);
                samples_sum+=samples;
                samples_sq_sum+=samples*samples;
            }
            VL = (samples_sq_sum-samples_sum*samples_sum/NL)/(NL-1);
            Vlvec[l_]=VL;
            Nlopt[l_]= GetNlOptimum(L,2,T,l_,Vlvec,epsilon);
            cout << "Initial Nlopt " << Nlopt[l_] << " VL " << VL << endl;
            for (int i=0;i<(Nlopt[l_]-NL);i++)       // extra samples if any
            {
                Model.GenerateSampleTerminalPairs(T,l_,2,SVec);
                PL=GetPayOff(SVec[1]);
                if (l_>=1) { PL_1=GetPayOff(SVec[0]);} else {PL_1=0;}
                samples =exp(-Model.r*T)*(PL-PL_1);
                samples_sum+=samples;
                samples_sq_sum+=samples*samples;
            }
            cout << "Done loop"  << endl;
            VL = (samples_sq_sum-samples_sum*samples_sum/Nlopt[l_])/(Nlopt[l_]-1);
            Vlvec[l_]=VL;
            cout << "VL " << VL << endl;
        }
        YL[L]=samples_sum/Nlopt[L];
        cout << "individual YL " <<YL[L] << endl;
        Y_est+=YL[L];
        if (L>=2)
        {
            cout << "Check convergence" << endl;
            sum_YL=0.0;
            for (int i=1;i <Nlopt[L];i++)
            {
                Model.GenerateSampleTerminalPairs(T,L,2,SVec);
                PL=GetPayOff(SVec[1]);
                PL_1=GetPayOff(SVec[0]);
                sum_YL+=exp(-Model.r*T)*(PL-PL_1);
            }
            cout << "Convergence value check " << fabs(sum_YL/Nlopt[L]) << " against " << (1/sqrt(2))*(M-1)*epsilon << endl;
            if (fabs(sum_YL/Nlopt[L]) < (1/sqrt(2))*(M-1)*epsilon) { Converged=true;}
        }
        if (Converged==false) {L+=1;}
    }
    cout << "Y est " << Y_est << endl;
}

double Option::PriceMC_Debias(HSModel Model,int M)		// Unbias MC
{
    double sum_CT=0.0;      // for Sum of PayOff
    double sum_CT2=0.0;     // for Sum of PayOff^2
    double Cn=0.0;          // asymptotically unbiased estimator fof Co
    double Qn=0.0;          // Survival probability

    double C_sum=0.0;
    double sum=0.0;
    double C1=0.0;
    double C2=0.0;

    double p=0.5;
    int s=2;
    int n=0;


    for (int j=0;j<M;j++)		// repeat M times
    {

    n = floor(log(RNDGenerator3.GetUniform())/log(1-p))+s+1;
    //cout << "n value picked is " << n << endl;	// Geometric random variable

    C_sum=0.0;
    Model.GenerateEstimatorDebias(T,s,K,Cn);
    C_sum=Cn;
    cout << "First term is " << C_sum << endl;
    for (int i=s+1; i<=n; i++)          // (..i=s+1. i<=n....)
    {
        Model.GenerateEstimatorDebias(T,i,K,Cn);
        C1=Cn;
        Model.GenerateEstimatorDebias(T,i-1,K,Cn);
        C2=Cn;
        Qn = pow(1-p,i-s);
        C_sum+=(C1-C2)/Qn;
    }
    sum=sum+C_sum;

    }

    return sum/M;
}

double EuropeanOption::GetPayOff(double ST)
{
    if (isCall==true) {
        return max(ST-K,0);
    } else {
        return max(K-ST,0);
    }
}
