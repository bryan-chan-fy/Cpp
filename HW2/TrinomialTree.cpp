#include "TrinomialTree.h"
#include <iostream>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;
using std::sqrt;
using namespace std;

// constructor to initialize Tree parameter values
template<class T> TriTree<T>::TriTree (int Steps_,double Spot_, double r_,double delta_, double Sigma_,double Time_): Steps(Steps_),Spot(Spot_),r(r_),delta(delta_), Sigma(Sigma_),Time(Time_)
{
}

// Build the Spot Trinomial Tree using additive (CRR) method
template<class T> void TriTree<T>::BuildTree()
{
    double thisTime;
    double nu = (r-delta-Sigma*Sigma/2);     // delta is dividend
    T delta_x;
    T base_xi;                  // lowest branch
    double delta_t;

    TreeBuilt=true;             // Indicator: Tree is built
    TheTree.resize(Steps+1);    // i layers
    delta_t=Time/Steps;
    delta_x=3/2*Sigma*sqrt(3*delta_t);  // delta_x >= Sigma * sqrt (3*delta_t) for convergent tree

    for (int i=0;i<=Steps; i++)
    {
        TheTree[i].resize(2*i+1); // resize correctly each ith layers

        thisTime=i*delta_t;
        base_xi=log(Spot)-i*delta_x;

        for (int j=0;j<=2*i;j++)
        {
            TheTree[i][j].first=base_xi+ j*delta_x;  // Generate Spot Tree
            // cout << " i:" <<i<< " j:"<<j<< " S:" << exp(TheTree[i][j].first) << endl;
        }
    }

}

template<class T>double TriTree<T>::GetPrice(const TreeProduct& TheProduct)
{
    T delta_x;
    T ContVal;
    double delta_t;
    double pu,pm,pd;        // probability up, probability mid, probability down
    double disfactor;       // discount factor for 1 time step
    double nu = (r-delta-Sigma*Sigma/2);     // delta is dividend
    int k;

    delta_t=Time/Steps;
    delta_x= 3/2*Sigma*sqrt(3*delta_t);    // delta_x > Sigma * sqrt (3*delta_t) for convergent tree
    pu = 0.5*((Sigma*Sigma*delta_t+nu*nu*delta_t*delta_t)/(delta_x*delta_x)+nu*delta_t/delta_x);
    pm = 1 - ((Sigma*Sigma*delta_t+nu*nu*delta_t*delta_t)/(delta_x*delta_x));
    pd = 0.5*((Sigma*Sigma*delta_t+nu*nu*delta_t*delta_t)/(delta_x*delta_x)-nu*delta_t/delta_x);
    // cout << "delta x:" << delta_x << endl;
    // cout << "Probabiliy pu: " << pu <<endl;
    // cout << "Probabiliy pm: " << pm <<endl;
    // cout << "Probabiliy pd: " << pd<<endl;

    disfactor=exp(-r*delta_t);
    // cout << "discount factor: " << disfactor <<endl;
    // cout << "-----" << endl;

    if (!TreeBuilt)
        BuildTree();

    // Build the Option Trinomial Tree (PayOff tree - not additive/log)
    // Calculate PayOff at i
    for (int j=0;j<=2*Steps;j++)
    {
        TheTree[Steps][j].second=TheProduct.FinalPayOff(GetNode(Steps,j,0));
        // cout << " i:" <<Steps<< " j:"<<j<< " S:" << TheTree[Steps][j].second << endl;
    }

    // Backward iteration discounting
    for (int i=Steps-1;i>=0;i--)
    {
        for (int j=0;j<=2*i;j++)
        {
            ContVal= disfactor* (pu*GetNode(i+1,j+2,1)+ pm*GetNode(i+1,j+1,1)+ pd*GetNode(i+1,j,1));  // Calculate Continuity Value
            // cout << "ContVal: " << ContVal << "  ";
            TheTree[i][j].second=TheProduct.SelectPayOff(GetNode(i,j,0),ContVal);
            // cout << " i:" <<i<< " j:"<<j<< " S:" << TheTree[i][j].second << endl;
        }
    }

    return TheTree[0][0].second;
}

template<class T> void TriTree<T>::DisplayTree(int whichTree)
{
    cout << setiosflags(ios::fixed) << setprecision(3);
    for (int i=0;i<=Steps;i++)
    {
        for (int j=0;j<=2*i;j++)
        {
            cout << setw(7) << GetNode(i,j,0);
            cout << endl;
        }
        cout << endl;
    }
}

// Get a node value from Tree
template<class T> double TriTree<T>::GetNode(int i,int j,int whichTree)
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


template class TriTree<double>; // This is for linking templates when separate files
