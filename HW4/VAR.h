#ifndef Var_h
#define Var_h

class PortfolioAsset
{
public:
    void GenerateAssetStock(int ndays,double X0,int M);
    void GenerateAssetTBill(int ndays,double Y0,int M);
    void GenerateAssetFX(int ndays,double Z0,int M);

};

#endif 
