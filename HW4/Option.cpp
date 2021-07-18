#include "Option.h"
#include "PayOff.h"

#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

double Option::PriceMC(BSModel Model,int M)		// Simple MC
{
    double sum_CT=0.0;      // for Sum of PayOff
    double sum_CT2=0.0;     // for Sum of PayOff^2
    SamplePath S(N);

    for (int j=0;j<M;j++)
    {
        Model.GenerateSamplePath(T,N,S);
        sum_CT=sum_CT+GetPayOff(S);
        sum_CT2=sum_CT2+GetPayOff(S)*GetPayOff(S);
    }
    cout << " sum CT CT2 " << sum_CT << " " << sum_CT2 << endl;
    SD_array[0]=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*Model.r*T)/(M-1));
    SD_array[1]=SD_array[0]/sqrt(M);
    return exp(-Model.r*T)*(1.0/M)*sum_CT;
}

double Option::PriceMCAntVar(BSModel Model,int M)	// Antithetic Variate MC
{
    double sum_CT=0.0;      // for Sum of PayOff
    double sum_CT2=0.0;     // for Sum of PayOff^2
    SamplePath Sa(N);
    SamplePath Sb(N);
    for (int j=0;j<M;j++)
    {
        Model.GenerateSamplePath(T,N,Sa,Sb);
        sum_CT=sum_CT+0.5*(GetPayOff(Sa)+GetPayOff(Sb));
        sum_CT2=sum_CT2+pow(0.5*(GetPayOff(Sa)+GetPayOff(Sb)),2);
    }
    cout << " sum CT CT2 " << sum_CT << " " << sum_CT2 << endl;
    SD_array[0]=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*Model.r*T)/(M-1));
    SD_array[1]=SD_array[0]/sqrt(M);
    return exp(-Model.r*T)*(1.0/M)*sum_CT;
}

double Option::PriceMCDeltaCV(BSModel Model,int M)	// Delta Control Variate MC
{
    double sum_CT=0.0;      // for Sum of PayOff
    double sum_CT2=0.0;     // for Sum of PayOff^2
    double dt=T/N;
    double erddt=exp((Model.r-Model.delta)*dt);
    double beta1=-1.0;
    double St;
    double CV;
    SamplePath S(N);

    for (int j=0;j<M;j++)
    {
        CV=0.0;
        St=Model.S0;
        Model.GenerateSamplePath(T,N,S);
        for (int i=0;i<N;i++)               // Compute CV based on generated Stock Path
        {
            CV = CV + Model.GetDelta(T-i*dt,K,St)*(S[i]-St*erddt);
            St=S[i];
        }
        sum_CT=sum_CT+(GetPayOff(S)+beta1*CV);
        sum_CT2=sum_CT2+pow((GetPayOff(S)+beta1*CV),2);
    }
    cout << " sum CT CT2 " << sum_CT << " " << sum_CT2 << endl;
    SD_array[0]=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*Model.r*T)/(M-1));
    SD_array[1]=SD_array[0]/sqrt(M);
    return exp(-Model.r*T)*(1.0/M)*sum_CT;
}

double Option::PriceMCAntCVCombined(BSModel Model,int M)	// Combined Antithetic & Delta CV MC
{
    double sum_CT=0.0;      // for Sum of PayOff
    double sum_CT2=0.0;     // for Sum of PayOff^2
    double dt=T/N;
    double erddt=exp((Model.r-Model.delta)*dt);
    double beta1=-1.0;
    double St1, St2;
    double CV1, CV2;
    double CT1, CT2;
    SamplePath Sa(N);
    SamplePath Sb(N);

    //Get CTvar(using random1) CTvar(using -random) + combine
    for (int j=0;j<M;j++)
    {
        CV1=0.0;
        CV2=0.0;
        CT1=0.0;
        CT2=0.0;
        St1=Model.S0;
        St2=Model.S0;
        Model.GenerateSamplePath(T,N,Sa,Sb);
        for (int i=0;i<N;i++)               // Compute CV based on generated Stock Path
        {
            CV1 = CV1 + Model.GetDelta(T-i*dt,K,St1)*(Sa[i]-St1*erddt);
            CV2 = CV2 + Model.GetDelta(T-i*dt,K,St2)*(Sb[i]-St2*erddt);
            St1=Sa[i];
            St2=Sb[i];
        }
        CT1=GetPayOff(Sa)+beta1*CV1;
        CT2=GetPayOff(Sb)+beta1*CV2;
        sum_CT=sum_CT+0.5*(CT1+CT2);
        sum_CT2=sum_CT2+pow(0.5*(CT1+CT2),2);
    }
    cout << " sum CT CT2 " << sum_CT << " " << sum_CT2 << endl;
    SD_array[0]=sqrt((sum_CT2-sum_CT*sum_CT/M)*exp(-2*Model.r*T)/(M-1));
    SD_array[1]=SD_array[0]/sqrt(M);
    return exp(-Model.r*T)*(1.0/M)*sum_CT;
}


double EuropeanOption::GetPayOff(SamplePath& S)
{
    double PayOff_T=0.0;

    if (isCall==true) {
        PayOff_T=PayOff::GetInstance().CallPayOff(S[N-1],K);
    } else {
        PayOff_T=PayOff::GetInstance().PutPayOff(S[N-1],K);
    }
    return PayOff_T;
}
