#include <iostream>
#include <chrono>

#include "PayOff.h"
#include "Random1.h"
#include "BSModel.h"
#include "Option.h"
#include "VAR.h"
#include "Cholesky.h"
#include "Basket.h"

using namespace std;

using ms=std::chrono::duration<double,std::milli>;

using std::cout;
using std::endl;

typedef double myarray[3][3];
//using myarray = double[3][3];

#define No_Days 100
#define No_Asset 3
#define No_Trial 850
typedef double mymatrix[No_Days][No_Trial][No_Asset];       // Define 3D matrix

int main()
{
    double S0=100.0, r=0.05, Sigma=0.25, Delta=0.02;
    double T=1, K=100.0;
    int N_array[2]={300,600};                               // N steps and M paths
    int M_array[2]={1000000,3000000};

    double CorrMat[][3]={{1.0,0.5,0.2},{0.5,1.0,-0.4},{0.2,-0.4,1.0}};  // Correlation Matrix
    double mu[3]={0.03,0.06,0.02};      // Parameters for the 3 assets
    double sigm[3]={0.05,0.2,0.15};
    double X0[3]={100.0,101.0,98.0};
    double a[3]={1.0/3,1.0/3,1.0/3};
    mymatrix X;

    PortfolioAsset Portfolio1;	    // For VAR

    BM_Asset ThreeAssets;           // Define the 3 assets
    double ThreeAssetPrice;

    BSModel Model(S0,r,Delta,Sigma);

    cout << "---BS Price ---" << endl;
    Model.GetOptionPrice(T,K);
    cout << endl;

    for (int k=1; k<2;k++)
    {
        int N=N_array[k];
        for (int l=1; l<2;l++)
        {
            int M=M_array[l];

            EuropeanOption EUCallOption(T,K,N,true);        // Instantiate Call/Put Options
            EuropeanOption EUPutOption(T,K,N,false);

            cout << "----MC Simple----" <<endl;		    // Monte Carlo - Warning: long sim. time
            const auto before1 = std::chrono::system_clock::now();
            cout << EUCallOption.PriceMC(Model,M) << " Call Price " << endl;
            cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array " << endl;
            cout << endl;
            cout << EUPutOption.PriceMC(Model,M) << " Put Price " << endl;
            cout << EUPutOption.SD_array[0] << " " << EUPutOption.SD_array[1] << " SD_array " << endl;
            const ms duration1 =std::chrono::system_clock::now()-before1;
            cout << "Each took average " << float(duration1.count())/2 << "ms" << endl << endl;


            cout << "----MC Anthetic Var----" <<endl;
            const auto before2 = std::chrono::system_clock::now();
            cout << EUCallOption.PriceMCAntVar(Model,M) << " Call Price " << endl;
            cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array " << endl;
            cout << endl;
            cout << EUPutOption.PriceMCAntVar(Model,M) << " Put Price " << endl;
            cout << EUPutOption.SD_array[0] << " " << EUPutOption.SD_array[1] << " SD_array " << endl;
            const ms duration2 =std::chrono::system_clock::now()-before2;
            cout << "Each took average " << float(duration2.count())/2 << "ms" << endl << endl;

            cout << "----MC Delta Control Var----" <<endl;
            const auto before3 = std::chrono::system_clock::now();
            cout << EUCallOption.PriceMCDeltaCV(Model,M) << " Call Price " << endl;
            cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array " << endl;
            cout << endl;
            cout << EUPutOption.PriceMCDeltaCV(Model,M) << " Put Price " << endl;
            cout << EUPutOption.SD_array[0] << " " << EUPutOption.SD_array[1] << " SD_array " << endl;
            const ms duration3=std::chrono::system_clock::now()-before3;
            cout << "Each took average " << float(duration3.count())/2 << "ms" << endl << endl;

            cout << "----MC Ant Var + Delta CV----" <<endl;
            const auto before4 = std::chrono::system_clock::now();
            cout << EUCallOption.PriceMCAntCVCombined(Model,M) << " Call Price " << endl;
            cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array " << endl;
            cout << endl;
            cout << EUPutOption.PriceMCAntCVCombined(Model,M) << " Put Price " << endl;
            cout << EUPutOption.SD_array[0] << " " << EUPutOption.SD_array[1] << " SD_array " << endl;
            const ms duration4=std::chrono::system_clock::now()-before4;
            cout << "Each took average " << float(duration4.count())/2 << "ms" << endl << endl;
        }
    }

    cout << "------ VAR & CVAR of Stock -----" << endl;
    Portfolio1.GenerateAssetStock(10,80,3000000);
    cout << "------ VAR & CVAR of T-bill -----" << endl;
    Portfolio1.GenerateAssetTBill(10,90000,3000000);
    cout << "------ VAR & CVAR of FX -----" << endl;
    Portfolio1.GenerateAssetFX(10,6.1,3000000);

    ThreeAssets.GenerateAssets(100,3,X0,mu,sigm,850,CorrMat,X);
    cout << " Basket Call Price:" << ThreeAssets.PriceAssetVanilla(100,3,1.0/365,850,X,a,0.05,100,true) << endl;
    cout << " Basket Put Price:" << ThreeAssets.PriceAssetVanilla(100,3,1.0/365,850,X,a,0.05,100,false) << endl;
    cout << " Basket Exotic Price:" << ThreeAssets.PriceAssetExotic(100,3,1.0/365,850,X,a,0.05,100) << endl;

    return 0;
}
