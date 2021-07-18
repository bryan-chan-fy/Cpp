#include <iostream>
#include "Heston.h"
#include "Options.h"

using std::cout;
using std::endl;

using namespace std;


int main()
{
    double T=1.0;
    double K=100;
    int N=600;
    int M=1000000;
    double price;

    HSModel HModel(100,0.010201,0.0319,0.61,6.21,0.019,-0.7);
    EuropeanOption EUCallOption(T,K,N,true);

    // Euler discretization
    price= EUCallOption.PriceMC(HModel,M,1);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC(HModel,M,2);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC(HModel,M,3);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC(HModel,M,4);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC(HModel,M,5);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";

    // Milhstein discretization
    price= EUCallOption.PriceMC_Milhstein(HModel,M,1);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC_Milhstein(HModel,M,2);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC_Milhstein(HModel,M,3);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC_Milhstein(HModel,M,4);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";
    price= EUCallOption.PriceMC_Milhstein(HModel,M,5);
    cout << "Price is " << price << endl;
    cout << EUCallOption.SD_array[0] << " " << EUCallOption.SD_array[1] << " SD_array ";

    // Multi-level Monte Carlo
    price=EUCallOption.PriceMLMC(HModel,2);

    // Debias Monte Carlo
    price=EUCallOption.PriceMC_Debias(HModel,10000);
    cout << "Price is " << price << endl;

    return 0;
}
