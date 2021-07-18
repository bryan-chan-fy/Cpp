#ifndef TRINOMIALTREE_H
#define TRINOMIALTREE_H

#include <iostream>
#include <vector>
#include "TreeProducts.h"

using std::vector;
using std::pair;

template<class T> class TriTree
{
private:
    bool TreeBuilt;
    int Steps;
    double Spot;
    double r;
    double Time;
    double delta_t;
    double Sigma;
    double delta;
    vector<vector<pair<T,T>>> TheTree;  // Pair is for 2 trees (Spot & Option), accessed by .First & .Second

public:
    TriTree (int Steps,double Spot,double r,double delta, double Sigma,double Time);
    void BuildTree();
    double GetNode(int i, int j,int whichTree);
    double GetPrice(const TreeProduct& TheProduct);
    void DisplayTree(int whichTree);

};

#endif
