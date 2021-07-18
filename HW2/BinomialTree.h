#ifndef BINOMIALTREE_H
#define BINOMIALTREE_H

#include <iostream>
#include <vector>
#include "TreeProducts.h"
#include "myenum.h"
#include "Options.h"

using std::vector;
using std::pair;


template<class T> class BinTree
{
public:
    BinTree (int Steps_,double Spot_,double r_,double delta_, double Sigma_,double Time_);
    void BuildTree();
    double GetNode(int i, int j,int whichTree);
    double GetPrice(const TreeProduct& TheProduct);
    double GetPriceBarrier_UO(const TreeProduct& TheProduct, double H_level_);
    double CalcModelPriceBarrier(double Strike_, double H_level_,BarrierType BarrType_);
    void DisplayTree(int whichTree);

private:
    bool TreeBuilt;
    int Steps;
    double Spot;
    double r;
    double delta;
    double Time;
    double delta_t;
    double Sigma;
    double nu=r-delta-Sigma*Sigma/2;
    vector<vector<pair<T,T>>> TheTree;  // Pair is for 2 trees (Spot & Option), accessed by .First & .Second

    BarrierOpt BarOption1;     // Adaptor Design Pattern
};

#endif // BINOMIALTREE_H
