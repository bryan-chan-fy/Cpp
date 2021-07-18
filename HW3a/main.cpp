#include "FiniteDifference.h"
#include "BSModel.h"
#include "Options.h"
#include "ExplicitMethod.h"
#include "ImplicitMethod.h"
#include "CrankNicolson.h"
#include "writecsv.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

using std::cout;
using std::endl;
using std::pair;
using std::string;

typedef vector<pair<string,vector<double>>> VectorPair;

VectorPair OutputDataset;		// For saving data to CSV

int main()
{
    double S0=100;  
    double r=0.06;
    double div=0.02; 
    double Vol=0.20;

    double K=100;  
    double T=1.0;
    double nu,dx,dt,N;
    vector<double> VolVec={0.05,0.10,0.15,0.20,0.25,0.30,0.35,0.40,0.45,0.50,0.55,0.60};
    vector<double> pu_impVec, pm_impVec, pd_impVec;
    double C1,C2;

    BSModel S_Model(S0,r,div,Vol);
    S_Model.GetOptionPrice(T,K);

    cout << "--- Call Option ---" <<endl;

    Call EuropeanCall(K,T,false);
    ParamFDM FDM_setup1 (&S_Model, &EuropeanCall);

    ExplicitMethod Exp_Method1(&FDM_setup1,11200,1407,0.0);       // using default DX=Sigma*Sqrt(3*dt)

    cout << Exp_Method1.SolvePDE() << " is the Explicit FDM Price " << endl;
    Exp_Method1.ClearArray();    // Clear some memory

    ImplicitMethod Imp_Method1(&FDM_setup1,11200,1407,0.0);       // using default DX=Sigma*Sqrt(3*dt)

    cout << Imp_Method1.SolvePDE() << " is the Implicit FDM Price " << endl;
    Imp_Method1.ClearArray();

    CrankNicolson CN_Method1(&FDM_setup1,106,1407,0.0);     // using default DX=Sigma*Sqrt(3*dt)

    cout << CN_Method1.SolvePDE() << " is the Crank Nicolson Price " << endl;
    CN_Method1.ClearArray();    // Clear some memory

    cout << "--- Put Option ---" << endl;

    Put EuropeanPut(K,T,false);
    ParamFDM FDM_setup2 (&S_Model, &EuropeanPut);

    ExplicitMethod Exp_Method2(&FDM_setup2,11200,1407,0.0); // using default DX=Sigma*Sqrt(3*dt)


    cout << Exp_Method2.SolvePDE() << " is the Explicit FDM Price " << endl;
    Exp_Method2.ClearArray();

    ImplicitMethod Imp_Method2(&FDM_setup2,11200,1407,0.0);   // using default DX=Sigma*Sqrt(3*dt)

    cout << Imp_Method2.SolvePDE() << " is the Implicit Price " << endl;
    Imp_Method2.ClearArray();

    CrankNicolson CN_Method2(&FDM_setup2,106,1407,0.0);       // using default DX=Sigma*Sqrt(3*dt)

    cout << CN_Method2.SolvePDE() << " is the Crank Nicolson Price " << endl;
    CN_Method2.ClearArray();

    // ----- This is for Iterative Trial out  -----

    Call EuropeanCall(K,T,false);
    Put EuropeanPut(K,T,false);

    ParamFDM FDM_setup3 (&S_Model, &EuropeanPut);

    ExplicitMethod Exp_Method3(&FDM_setup3,15000,1000,0.002);

    cout << Exp_Method3.SolvePDE() << " is the Explicit FDM Price " << endl;
    Exp_Method3.ClearArray();

    ImplicitMethod Imp_Method3(&FDM_setup3,12000,3000,0.0015);

    cout << Imp_Method3.SolvePDE() << " is the Implicit FDM Price " << endl;
    Imp_Method3.ClearArray();

    CrankNicolson CN_Method3(&FDM_setup3,200,2000,0.002);

    cout << CN_Method3.SolvePDE() << " is the Crank Nicolson Price " << endl;
    CN_Method3.ClearArray();

    // computing pu, pm, pd for different volatilities
    cout << VolVec.size() << endl;

    N=11200;
    dt=T/N;
    dx=0.0032733;
    for (int i=0;i<VolVec.size();i++)
    {
        Vol=VolVec[i];
        nu = r-div-Vol*Vol/2;
        pu_impVec.push_back(-dt*(Vol*Vol/(2*dx*dx)+nu/(2*dx)));
        pm_impVec.push_back(1+dt*Vol*Vol/(dx*dx)+r*dt);
        pd_impVec.push_back(-dt*(Vol*Vol/(2*dx*dx)-nu/(2*dx)));
        // cout << pu_impVec[i] << " ";
    }

    OutputDataset = {{"Sigma",VolVec},{"pu_imp",pu_impVec},{"pm_imp",pm_impVec},{"pd_imp",pd_impVec}};
    write_csv("impFDM_Data.csv",OutputDataset);       // write data to CSV file for R plotting

    // Greeks from Explicit FDM

    S_Model.GetGreeks(T,K);
    ParamFDM FDM_setup4 (&S_Model, &EuropeanCall);

    ExplicitMethod Exp_Method4(&FDM_setup4,11200,1407,0.0);

    cout << Exp_Method4.SolvePDE() << " is the Explicit FDM Price " << endl;

    cout << "The Delta is " << Exp_Method4.getDelta() << endl;
    cout << "The Gamma is " << Exp_Method4.getGamma() << endl;
    cout << "The Theta is " << Exp_Method4.getTheta() << endl;
    Exp_Method4.ClearArray();

    BSModel S_Model_5(S0,r,div,Vol+0.01);
    ParamFDM FDM_setup5 (&S_Model_5, &EuropeanCall);
    ExplicitMethod Exp_Method5(&FDM_setup5,11200,1407,0.0);
    C1 = Exp_Method5.SolvePDE();

    BSModel S_Model_6(S0,r,div,Vol-0.01);
    ParamFDM FDM_setup6 (&S_Model_6, &EuropeanCall);
    ExplicitMethod Exp_Method6(&FDM_setup6,11200,1407,0.0);
    C2 = Exp_Method6.SolvePDE();

    cout << "The Vega is " << (C1-C2)/(2*0.01) << endl;

    return 0;
}
