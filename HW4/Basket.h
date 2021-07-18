#ifndef Basket_h
#define Basket_h

#define No_Days 100
#define No_Asset 3
#define No_Trial 850
typedef double mymatrix[No_Days][No_Trial][No_Asset];

class BM_Asset
{
public:
    void GenerateAssets(int ndays,int num_asset, double X0[3],double mu[3],double sigm[3],int M, double CorrMat[][3],mymatrix& X);
    double PriceAssetVanilla(int ndays, int num_asset, double dt,int M, mymatrix& X, double a[3],double r, double K,bool isCall);
    double PriceAssetExotic(int ndays, int num_asset, double dt, int M, mymatrix& X, double a[3],double r, double K);
private:

};

#endif //
