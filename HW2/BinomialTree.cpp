#include "BinomialTree.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include "myenum.h"

using std::cout;
using std::endl;
using std::sqrt;
using namespace std;

// constructor to initialize Tree parameter values
template<class T> BinTree<T>::BinTree (int Steps_,double Spot_, double r_,double delta_, double Sigma_,double Time_): Steps(Steps_),Spot(Spot_),r(r_),delta(delta_),Sigma(Sigma_),Time(Time_)
{
}

// Build the Spot Binomial Tree using additive (CRR) method
template<class T> void BinTree<T>::BuildTree()
{
    double thisTime;
    T delta_x;
    T base_xi;                  // lowest branch
    double delta_t;
    double nu;

    TreeBuilt=true;             // Indicator: Tree is built
    TheTree.resize(Steps+1);    // i layers
    delta_t=Time/Steps;
    nu=r-delta-Sigma*Sigma/2;    // delta is dividend
    delta_x=sqrt(pow(nu,2)*delta_t*delta_t+Sigma*Sigma*delta_t);

    for (int i=0;i<=Steps; i++)
    {
        TheTree[i].resize(i+1); // resize correctly each ith layers

        thisTime=i*delta_t;
        base_xi=log(Spot)-i*delta_x;

        for (int j=0;j<=i;j++)
        {
            TheTree[i][j].first=base_xi+ 2*j*delta_x;  // Generate Spot Tree
            // cout << " i:" <<i<< " j:"<<j<< " S:" << exp(TheTree[i][j].first) << endl;
        }
    }

}

template<class T>double BinTree<T>::GetPrice(const TreeProduct& TheProduct)
{
    T delta_x;
    T ContVal;
    double delta_t;
    double pu,pd;           // probability up, probability down
    double disfactor;       // discount factor for 1 time step
    int k;
    double nu;

    delta_t=Time/Steps;
    nu=r-delta-Sigma*Sigma/2;      // delta is dividend
    delta_x=sqrt(pow(nu,2)*delta_t*delta_t+Sigma*Sigma*delta_t);
    pu = 0.5 + 0.5*(nu)*delta_t/delta_x;
    pd = 1-pu;
    // cout << "delta x:" << delta_x << endl;
    // cout << "Probabiliy pu: " << pu <<endl;
    // cout << "Probabiliy pd: " << pd<<endl;

    disfactor=exp(-r*delta_t);
    // cout << "discount factor: " << disfactor <<endl;

    if (!TreeBuilt)
        BuildTree();

    // Build the Option Binomial Tree (PayOff tree - not additive/log)
    // Calculate PayOff at i
    for (int j=0;j<=Steps;j++)
    {
        TheTree[Steps][j].second=TheProduct.FinalPayOff(GetNode(Steps,j,0));
        // cout << " i:" <<Steps<< " j:"<<j<< " S:" << TheTree[Steps][j].second << endl;
    }

    // Backward iteration discounting
    for (int i=Steps-1;i>=0;i--)
    {
        for (int j=0;j<=i;j++)
        {
            ContVal= disfactor* (pu*GetNode(i+1,j+1,1)+ pd*GetNode(i+1,j,1));  // Calculate Continuity Value
            // cout << "ContVal: " << ContVal << "  ";
            TheTree[i][j].second=TheProduct.SelectPayOff(GetNode(i,j,0),ContVal);
            // cout << " i:" <<i<< " j:"<<j<< " S:" << TheTree[i][j].second << endl;
        }
    }

    return TheTree[0][0].second;
}

template<class T>double BinTree<T>::GetPriceBarrier_UO(const TreeProduct& TheProduct, double H_level)
{
    T delta_x;
    T ContVal;
    double delta_t;
    double pu,pd;           // probability up, probability down
    double disfactor;       // discount factor for 1 time step
    int k;
    int Indicator;

    delta_t=Time/Steps;
    nu=r-delta-Sigma*Sigma/2;
    delta_x=sqrt(pow(nu,2)*delta_t*delta_t+Sigma*Sigma*delta_t);
    pu = 0.5 + 0.5*(nu)*delta_t/delta_x;
    pd = 1-pu;

    disfactor=exp(-r*delta_t);

    if (!TreeBuilt)
        BuildTree();

    // Build the Barrier Option Binomial Tree (PayOff tree - not additive/log)
    // Calculate PayOff at i
    for (int j=0;j<=Steps;j++)
    {
        Indicator=BarOption1.UO_Indicator(GetNode(Steps,j,0),H_level);

        TheTree[Steps][j].second=TheProduct.FinalPayOff(GetNode(Steps,j,0))*Indicator;
        // cout << " i:" <<Steps<< " j:"<<j<< " S:" << GetNode(Steps,j,0) << " payoff "<< TheTree[Steps][j].second << endl;
    }

    // Backward iteration discounting
    for (int i=Steps-1;i>=0;i--)
    {
        for (int j=0;j<=i;j++)
        {
            ContVal= disfactor* (pu*GetNode(i+1,j+1,1)+ pd*GetNode(i+1,j,1));  // Calculate Continuity Value
            // cout << "ContVal: " << ContVal << "  ";

            Indicator=BarOption1.UO_Indicator(GetNode(i,j,0),H_level);

            TheTree[i][j].second=TheProduct.SelectPayOff(GetNode(i,j,0),ContVal)*Indicator;
            // cout << " i:" <<i<< " j:"<<j<< " S:" << TheTree[i][j].second << endl;
        }
    }

    return TheTree[0][0].second;
}

// Calculate Theoretical Barrier Option Price based on Model/Formula
template<class T>double BinTree<T>::CalcModelPriceBarrier(double Strike,double H_level,BarrierType BarrType)
{
    BarrierOpt BarOptionM1(Spot, r, delta, Sigma, Time);    // Setup Barrier Option Parameters
    switch (BarrType)
    {
    case UI:
        return BarOptionM1.GetUI_CallPrice(Strike,H_level);	// Get the UI theoretical Call Price
    case UO:
        return BarOptionM1.GetUO_CallPrice(Strike,H_level);	// Get the UO theoretical Call Price
    }
}

// For display Tree - checking purpose
template<class T> void BinTree<T>::DisplayTree(int whichTree)
{
    cout << setiosflags(ios::fixed) << setprecision(3);
    for (int i=0;i<=Steps;i++)
    {
        for (int j=0;j<=i;j++)
        {
            cout << setw(7) << GetNode(i,j,0);
            cout << endl;
        }
        cout << endl;
    }
}

// Get a node value from Tree
template<class T> double BinTree<T>::GetNode(int i,int j,int whichTree)
{
    if (!TreeBuilt)
        BuildTree();
    if (whichTree==0)       // Get node from First tree  (additive/log)
        return exp(TheTree[i][j].first);
    else if (whichTree==1)  // Get node from Second Tree (non-additive/non-log)
        return TheTree[i][j].second;
    else
        return -1.0;
}




template class BinTree<double>; // This is for linking templates when separate files

//}
