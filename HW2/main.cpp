#include <iostream>
#include "Wrapper.h"
#include "BinomialTree.h"
#include "Options.h"
#include "TreeProducts.h"
#include "myenum.h"
#include "TrinomialTree.h"

#include "Parameters.h"
#include "BSModel01.h"
#include "Secant.h"
#include <cstring>
#include <vector>
#include <iostream>


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::pair;

using namespace std;
using namespace myenum;

typedef vector<pair<string,vector<double>>> VectorPair;

int main()
{
    int numSteps;               // Binomial or Trinomial Tree Parameters
    double SpotPrice;
    double r_int;
    double Maturity;
    double Sigma;
    double K1;
    double div;                 // dividend
    double H;                   // Barrier Level

    double BinTree_EU_CallPrice[15],BinTree_EU_PutPrice[15];   // Store Binomial Tree Option Price
    double BinTree_US_CallPrice[15],BinTree_US_PutPrice[15];
    double BinTree_EU_PutPrice_Steps[15][10];
    double BinTree_EU_PutPrice_Error[15][10];                   // store Binomial tree computation error
    double TriTree_EU_PutPrice_Steps[10];
    double TriTree_EU_PutPrice_Error[10];                       // store Trinomial tree computation error
    double NSteps[10]={10,30,50,100,150,200,250,300,350,400};   // Store number of steps for computation error check

    double TriTree_EU_CallPrice,TriTree_EU_PutPrice;   // Store Trinomial Tree Option Price
    double TriTree_US_CallPrice,TriTree_US_PutPrice;

    double SpotEq[2]; // j=0 Day 1; j=1 Day 2
    string filenameEq;
    // TbData_xxx [j][i]  j=day ; i=0 contract name; j=1 Implied Volatility
    string TbData_Eq_Call[15][2]; string TbData_Eq_Put[15][2];
    double Expiry;  // contract Expiry
    double Price_CallOption_Eq[15],Price_PutOption_Eq[15];  // Store Contract Call/Put Price
    double Price_CallOptionD2_Eq[15],Price_PutOptionD2_Eq[15];  // Store Contract Call/Put Price
    double CallPrice,PutPrice;
    double CallPriceD2,PutPriceD2;
    double StrikeEq;                                        // Contract Strike Price
    double SigmaCall;
    double SigmaPut;
    double r[2];                                             // short term interest rates for 2 days
    double Tolerance=1e-6;
    double Price_CallOptionD2Calc_Eq[15], Price_PutOptionD2Calc_Eq[15]; // store calculated Day 2 Option Price
    double Price_CallOptionCalc_Eq, Price_PutOptionCalc_Eq;  // calculated B.Schole Option Prices
    double TbAvg[15];                                        // Average Call/Put Implied volatility
    double AvgSigma[15];
    double Strike_array[15], Expiry_array[15];
    double BinTree_CallImpVol[15],BinTree_PutImpVol[15];     // Implied Volatility from Binomial Tree
    double BS_CallImpVolD2[15],BS_PutImpVolD2[15];          // Implied VOlatility Day 2 using Black Schole

    vector<double> vecT;                            // establish vectors to store for writing to csv file
    vector<double> vecStrikeEq;
    vector<double> vecAvgSigma;
    vector<double> vecBSCallPrice;
    vector<double> vecBSPutPrice;
    vector<double> vecBinTreeEUCallPrice;
    vector<double> vecBinTreeEUPutPrice;
    vector<double> vecBinTreeUSCallPrice;
    vector<double> vecBinTreeUSPutPrice;
    VectorPair OutputDataset;
    vector<double> vecBinTreeError1; vector<double> vecBinTreeError2;
    vector<double> vecBinTreeError3; vector<double> vecBinTreeError4;
    vector<double> vecBinTreeError5; vector<double> vecBinTreeError6;
    vector<double> vecBinTreeError7; vector<double> vecBinTreeError8;
    vector<double> vecBinTreeError9; vector<double> vecBinTreeError10;
    vector<double> vecTriTreeError;


    // Contract names
    TbData_Eq_Call[0][0]="AMZN201106C02900000.Close"; TbData_Eq_Put[0][0]="AMZN201106P02900000.Close";
    TbData_Eq_Call[1][0]="AMZN201106C03000000.Close"; TbData_Eq_Put[1][0]="AMZN201106P03000000.Close";
    TbData_Eq_Call[2][0]="AMZN201106C03100000.Close"; TbData_Eq_Put[2][0]="AMZN201106P03100000.Close";
    TbData_Eq_Call[3][0]="AMZN201106C03150000.Close"; TbData_Eq_Put[3][0]="AMZN201106P03150000.Close";
    TbData_Eq_Call[4][0]="AMZN201106C03200000.Close"; TbData_Eq_Put[4][0]="AMZN201106P03200000.Close";
    TbData_Eq_Call[5][0]="AMZN201218C02900000.Close"; TbData_Eq_Put[5][0]="AMZN201218P02900000.Close";
    TbData_Eq_Call[6][0]="AMZN201218C03000000.Close"; TbData_Eq_Put[6][0]="AMZN201218P03000000.Close";
    TbData_Eq_Call[7][0]="AMZN201218C03100000.Close"; TbData_Eq_Put[7][0]="AMZN201218P03100000.Close";
    TbData_Eq_Call[8][0]="AMZN201218C03150000.Close"; TbData_Eq_Put[8][0]="AMZN201218P03150000.Close";
    TbData_Eq_Call[9][0]="AMZN201218C03200000.Close"; TbData_Eq_Put[9][0]="AMZN201218P03200000.Close";
    TbData_Eq_Call[10][0]="AMZN210115C02900000.Close"; TbData_Eq_Put[10][0]="AMZN210115P02900000.Close";
    TbData_Eq_Call[11][0]="AMZN210115C03000000.Close"; TbData_Eq_Put[11][0]="AMZN210115P03000000.Close";
    TbData_Eq_Call[12][0]="AMZN210115C03100000.Close"; TbData_Eq_Put[12][0]="AMZN210115P03100000.Close";
    TbData_Eq_Call[13][0]="AMZN210115C03150000.Close"; TbData_Eq_Put[13][0]="AMZN210115P03150000.Close";
    TbData_Eq_Call[14][0]="AMZN210115C03200000.Close"; TbData_Eq_Put[14][0]="AMZN210115P03200000.Close";


    Parameters MyParameter1;
    // Extract Spot prices
    for (int j=0;j<2;j++)
    {
        SpotEq[j]=MyParameter1.GetValue("yahoo_fdatax1_norow.csv","AMZN.Close",j);
        cout << SpotEq[j] << " Extracted Spot Price Equity for Day " << j+1 << endl;
    }

    // Extract contract Call / Put option prices.
    // Sep 30 2020 - Nov 06 2020= 28 business days
    // Sep 30 2020 - Dec 18 2020= 58 business days
    // Sep 30 2020 - Jan 15 2021= 78 business days
    r[0]=0.09/100;
    r[1]=0.09/100;
    int i=0;
    while (i<15){
        if (i<5){
            filenameEq="yahoo_fdatax2_norow.csv";
            Expiry=28.0/252;        // 0. decimal is necessary
        }
        if (i>=5 && i<10){
            filenameEq="yahoo_fdatax2b_norow.csv";
            Expiry=58.0/252;        // 0. decimal is necessary
        }
        if (i>=10 && i<15){
            filenameEq="yahoo_fdatax2c_norow.csv";
            Expiry=78.0/252;        // 0. decimal is necessary
        }
        if (i%5==0){
            StrikeEq=2900.0;
        }
        if (i%5==1){
            StrikeEq=3000.0;
        }
        if (i%5==2){
            StrikeEq=3100.0;
        }
        if (i%5==3){
            StrikeEq=3150.0;
        }
        if (i%5==4){
            StrikeEq=3200.0;
        }
        Strike_array[i]=StrikeEq;   // Store the Strike & Expiry with index
        Expiry_array[i]=Expiry;

        CallPrice=MyParameter1.GetValue(filenameEq,TbData_Eq_Call[i][0],0);  // Extract Day 1 Option Prices
        PutPrice=MyParameter1.GetValue(filenameEq,TbData_Eq_Put[i][0],0);
        Price_CallOption_Eq[i]=CallPrice;
        Price_PutOption_Eq[i]=PutPrice;
        // cout << CallPrice << " Extracted Call Price for Eq " << "  ";
        // cout << PutPrice << " Extracted Put Price for Eq" <<endl;

        BS_Call myCall1(SpotEq[0],StrikeEq,r[0],0,Expiry);    // Setup the Option parameters for Day 1
        BS_Put myPut1(SpotEq[0],StrikeEq,r[0],0,Expiry);

        SigmaCall = Secant(CallPrice,1.0,0.01,Tolerance,myCall1); // Calculate implied volatility
        SigmaPut = Secant(PutPrice,1.0,0.01,Tolerance,myPut1);

        TbData_Eq_Call[i][1]=std::to_string(SigmaCall);     // Store implied volatility
        TbData_Eq_Put[i][1]=std::to_string(SigmaPut);
        TbAvg[i] = (std::stod(TbData_Eq_Call[i][1])+std::stod(TbData_Eq_Put[i][1]))/2; // Average Call/Put volatility

        CallPriceD2=MyParameter1.GetValue(filenameEq,TbData_Eq_Call[i][0],1);  // Extract Day 2 Option Price
        PutPriceD2=MyParameter1.GetValue(filenameEq,TbData_Eq_Put[i][0],1);
        Price_CallOptionD2_Eq[i]=CallPriceD2;
        Price_PutOptionD2_Eq[i]=PutPriceD2;

        i++;
    }

    for (int i=0;i<15;i++) {
        if (i<5) {AvgSigma[i]=(TbAvg[0]+TbAvg[1]+TbAvg[2]+TbAvg[3]+TbAvg[4])/5;} // Avg implied volaility for each Maturity
        if (i>=5 && i<10) {AvgSigma[i]=(TbAvg[5]+TbAvg[6]+TbAvg[7]+TbAvg[8]+TbAvg[9])/5;}
        if (i>=10 && i<15) {AvgSigma[i]=(TbAvg[10]+TbAvg[11]+TbAvg[12]+TbAvg[13]+TbAvg[14])/5;}
        cout << AvgSigma[i] << endl;
    }

    for (int i=0;i<15;i++)
    {
        BS_Call myCall2(SpotEq[1],Strike_array[i],r[1],0,Expiry_array[i]);    // Setup the Option parameters for Day 2
        BS_Put myPut2(SpotEq[1],Strike_array[i],r[1],0,Expiry_array[i]);

        Price_CallOptionD2Calc_Eq[i]=myCall2(AvgSigma[i]); // Calculate Day 2 Option Prices using Avg imp. vol.
        Price_PutOptionD2Calc_Eq[i]=myPut2(AvgSigma[i]);
    }

    cout << "-----Display Day 1 Implied Volatility -----" << endl;
    cout << "< Call         < Put" << endl;
    for (int i=0;i<15;i++) {
        cout << TbData_Eq_Call[i][1] << "   "<< TbData_Eq_Put[i][1] << endl;
    }

    cout << "-----Display Black Schole Calculated Day 2 Call & Put Option Prices -----" << endl;
    cout << "< Call--Calc Actual  < Put--Calc  Actual" << endl;
    for (int i=0;i<15;i++) {
        cout << Price_CallOptionD2Calc_Eq[i] << "   "<< Price_CallOptionD2_Eq[i] << "        " << Price_PutOptionD2Calc_Eq[i] << "   "<< Price_PutOptionD2_Eq[i] << endl;
    }

    r_int = r[1];
    numSteps=200;
    SpotPrice=SpotEq[1];        // Day 2 Spot Price

    cout << "---- Binomial Tree ---- " << endl;     // Day 2 Option Price using binomial tree, Day 1 average implied volatility
    cout << "< EU--Call  Put    < US --Call  Put" << endl;

    for (int i=0;i<15;i++)
    {

        Maturity=Expiry_array[i];
        Sigma=AvgSigma[i];
        K1=Strike_array[i];  // K1=40.0;

        BinTree<double> PriceTree(numSteps,SpotPrice,r_int,0,Sigma,Maturity);  // Intialize for a binomial tree object 'PriceTree'

        PriceTree.BuildTree();

        TreeEuropean europeanOption1(K1,call);
        BinTree_EU_CallPrice[i]=PriceTree.GetPrice(europeanOption1);
        // cout << "European Call with Strike "<< K1 << " is "<< BinTree_EU_CallPrice[i] << endl;

        TreeEuropean europeanOption2(K1,put);
        BinTree_EU_PutPrice[i]=PriceTree.GetPrice(europeanOption2);
        // cout << "European Put with Strike "<< K1 << " is "<< BinTree_EU_PutPrice[i] << endl;

        TreeAmerican americanOption1(K1,call);
        BinTree_US_CallPrice[i]=PriceTree.GetPrice(americanOption1);
        // cout << "American Call with Strike "<< K1 << " is "<< BinTree_US_CallPrice[i] << endl;

        TreeAmerican americanOption2(K1,put);
        BinTree_US_PutPrice[i]=PriceTree.GetPrice(americanOption2);
        // cout << "American Put with Strike "<< K1 << " is "<< BinTree_US_PutPrice[i] << endl;

        cout << BinTree_EU_CallPrice[i] << "   " << BinTree_EU_PutPrice[i] << "   " << BinTree_US_CallPrice[i] << "  " << BinTree_US_PutPrice[i] << endl;
    }

    cout << "Implied Volatility from US Binomial Tree vs. BS model derived actual Implied Volatility" << endl;
    cout << " < Call Otion-BinTree  BSModel  < Put Option-BinTree  BSModel" << endl;

    // Binomial Tree Implied Volatility and Day 2 actual Implied Volatilty
    for (int i=0;i<15;i++)
    {

        BS_Call myCall2(SpotEq[1],Strike_array[i],r[1],0,Expiry_array[i]);    // Setup the Option parameters for Day 2
        BS_Put myPut2(SpotEq[1],Strike_array[i],r[1],0,Expiry_array[i]);

        SigmaCall = Secant(BinTree_US_CallPrice[i],1.0,0.01,Tolerance,myCall2); // Calculate implied volatility using Binomial Price
        SigmaPut = Secant(BinTree_US_PutPrice[i],1.0,0.01,Tolerance,myPut2);

        BinTree_CallImpVol[i]=SigmaCall;     // Store implied volatility
        BinTree_PutImpVol[i]=SigmaPut;

        SigmaCall = Secant(Price_CallOptionD2_Eq[i],1.0,0.01,Tolerance,myCall2); // Calculate implied volatility using Binomial Price
        SigmaPut = Secant(Price_PutOptionD2_Eq[i],1.0,0.01,Tolerance,myPut2);

        BS_CallImpVolD2[i]=SigmaCall; // Calculate Day 2 Option Prices using Avg imp. vol.
        BS_PutImpVolD2[i]=SigmaPut;

        cout << BinTree_CallImpVol[i] << "  " << BS_CallImpVolD2[i] << " " << BinTree_PutImpVol[i] << " " << BS_PutImpVolD2[i] << endl;
    }

    cout << "--- Computation for error checking --- " << endl;

    // Computation error check (for Put Price only) - Binomial Tree
    for (int i=0;i<15;i++)
    {
        Maturity=Expiry_array[i];
        Sigma=AvgSigma[i];
        K1=Strike_array[i];
        r_int = r[1];
        SpotPrice=SpotEq[1];

        for (int j=0;j<10;j++)      // Looping different N steps
        {
            numSteps=NSteps[j];

            BinTree<double> PriceTreeX(numSteps,SpotPrice,r_int,0,Sigma,Maturity);  // Intialize for a binomial tree object 'PriceTreeX'

            PriceTreeX.BuildTree();

            TreeEuropean europeanOptionX(K1,put);
            BinTree_EU_PutPrice_Steps[i][j]=PriceTreeX.GetPrice(europeanOptionX);
            BinTree_EU_PutPrice_Error[i][j]=Price_PutOptionD2Calc_Eq[i]-BinTree_EU_PutPrice_Steps[i][j]; // error = B.Schole Price - Bin. Tree Price of N steps
            // cout << Price_PutOptionD2Calc_Eq[i] << " ";
            // cout << BinTree_EU_PutPrice_Steps[i][j] << " ";
            cout << BinTree_EU_PutPrice_Error[i][j] << " ";
        }
        cout << endl;
    }

    cout << "---- Trinomial Tree (real contract) ---- " << endl;     // Day 2 Option Price using Trinomial tree, Day 1 average implied volatility
    cout << "< EU--Call  Put    < US --Call  Put" << endl;

    r_int = r[1];
    numSteps=200;
    SpotPrice=SpotEq[1];        // Day 2 Spot Price
    for (int i=0;i<15;i++)
    {

        Maturity=Expiry_array[i];
        Sigma=AvgSigma[i];
        K1=Strike_array[i];  // K1=40.0;
        div=0;

        TriTree<double> PriceTree(numSteps,SpotPrice,r_int,div, Sigma,Maturity);  // Intialize for a trinomial tree object 'PriceTree'

        PriceTree.BuildTree();

        TreeEuropean europeanOption1(K1,call);
        TriTree_EU_CallPrice=PriceTree.GetPrice(europeanOption1);
        // cout << "European Call with Strike "<< K1 << " is "<< BinTree_EU_CallPrice << endl;

        TreeEuropean europeanOption2(K1,put);
        TriTree_EU_PutPrice=PriceTree.GetPrice(europeanOption2);
        // cout << "European Put with Strike "<< K1 << " is "<< BinTree_EU_PutPrice << endl;

        TreeAmerican americanOption1(K1,call);
        TriTree_US_CallPrice=PriceTree.GetPrice(americanOption1);
        // cout << "American Call with Strike "<< K1 << " is "<< BinTree_US_CallPrice << endl;

        TreeAmerican americanOption2(K1,put);
        TriTree_US_PutPrice=PriceTree.GetPrice(americanOption2);
        // cout << "American Put with Strike "<< K1 << " is "<< BinTree_US_PutPrice << endl;

        cout << TriTree_EU_CallPrice << "   " << TriTree_EU_PutPrice << "   " << TriTree_US_CallPrice << "  " << TriTree_US_PutPrice << endl;
    }

    // Repeat Trinomial Tree Using Specified Values
    cout << "---- Trinomial Tree (specified values) ---- " << endl;
    cout << "< EU--Call  Put    < US --Call  Put" << endl;

    r_int = 0.05;
    numSteps=200;
    SpotPrice=100;
    Maturity=1;
    Sigma=0.20;
    K1=100;
    div=0.02;

    TriTree<double> PriceTree(numSteps,SpotPrice,r_int,div, Sigma,Maturity);  // Intialize for a trinomial tree object 'PriceTree'

    PriceTree.BuildTree();

    TreeEuropean europeanOption1(K1,call);
    TriTree_EU_CallPrice=PriceTree.GetPrice(europeanOption1);

    TreeEuropean europeanOption2(K1,put);
    TriTree_EU_PutPrice=PriceTree.GetPrice(europeanOption2);

    TreeAmerican americanOption1(K1,call);
    TriTree_US_CallPrice=PriceTree.GetPrice(americanOption1);

    TreeAmerican americanOption2(K1,put);
    TriTree_US_PutPrice=PriceTree.GetPrice(americanOption2);

    cout << TriTree_EU_CallPrice << "   " << TriTree_EU_PutPrice << "   " << TriTree_US_CallPrice << "  " << TriTree_US_PutPrice << endl;

    BS_Call myCall1(SpotPrice,K1,r_int,div,Maturity);    // Setup the Option parameters
    BS_Put myPut1(SpotPrice,K1,r_int,div,Maturity);
    Price_CallOptionCalc_Eq=myCall1(Sigma); // Calculate Option Price using B. Schole Model
    Price_PutOptionCalc_Eq=myPut1(Sigma);
    cout << "BS Call Price: " << Price_CallOptionCalc_Eq << "   BS Put Price: " << Price_PutOptionCalc_Eq << endl;

    // Computation error check (for Put Price only) - Trinomial Tree using Specified Values
    for (int j=0;j<10;j++)      // Looping different N steps
    {
        numSteps=NSteps[j];

        TriTree<double> PriceTreeX(numSteps,SpotPrice,r_int,div, Sigma,Maturity);  // Initialize for a Trinomial tree object 'PriceTreeX'

        PriceTreeX.BuildTree();

        TreeEuropean europeanOptionX(K1,put);
        TriTree_EU_PutPrice_Steps[j]=PriceTreeX.GetPrice(europeanOptionX);
        TriTree_EU_PutPrice_Error[j]=Price_PutOptionCalc_Eq-TriTree_EU_PutPrice_Steps[j]; // error = B.Schole Price - Tri. Tree Price of N steps
        cout << TriTree_EU_PutPrice_Error[j] << " ";

        vecTriTreeError.push_back(TriTree_EU_PutPrice_Error[j]);  // push error data into vector for csv file writting
    }
    cout << endl;
    OutputDataset = {{"TriTree Error",vecTriTreeError}};
    MyParameter1.WriteValue("TriTreeResult.csv",OutputDataset);


    // Convert data to vector to write to csv file for R plotting
    for (int i=0;i<15;i++)
    {
        vecT.push_back(Expiry_array[i]);
        vecStrikeEq.push_back(Strike_array[i]);
        vecAvgSigma.push_back(AvgSigma[i]);
        vecBSCallPrice.push_back(Price_CallOptionD2Calc_Eq[i]);
        vecBSPutPrice.push_back(Price_PutOptionD2Calc_Eq[i]);
        vecBinTreeEUCallPrice.push_back(BinTree_EU_CallPrice[i]);
        vecBinTreeEUPutPrice.push_back(BinTree_EU_PutPrice[i]);
        vecBinTreeUSCallPrice.push_back(BinTree_US_CallPrice[i]);
        vecBinTreeUSPutPrice.push_back(BinTree_US_PutPrice[i]);
        vecBinTreeError1.push_back(BinTree_EU_PutPrice_Error[i][0]);
        vecBinTreeError2.push_back(BinTree_EU_PutPrice_Error[i][1]);
        vecBinTreeError3.push_back(BinTree_EU_PutPrice_Error[i][2]);
        vecBinTreeError4.push_back(BinTree_EU_PutPrice_Error[i][3]);
        vecBinTreeError5.push_back(BinTree_EU_PutPrice_Error[i][4]);
        vecBinTreeError6.push_back(BinTree_EU_PutPrice_Error[i][5]);
        vecBinTreeError7.push_back(BinTree_EU_PutPrice_Error[i][6]);
        vecBinTreeError8.push_back(BinTree_EU_PutPrice_Error[i][7]);
        vecBinTreeError9.push_back(BinTree_EU_PutPrice_Error[i][8]);
        vecBinTreeError10.push_back(BinTree_EU_PutPrice_Error[i][9]);
    }

    OutputDataset = {{"Expiry",vecT},{"Strike Eq",vecStrikeEq},{"Avg Imp Vol",vecAvgSigma},{"BS Call Price",vecBSCallPrice},{"BS Put Price",vecBSPutPrice},{"BinTree EU CallPrice",vecBinTreeEUCallPrice},{"BinTree EU PutPrice",vecBinTreeEUPutPrice},{"BinTree US CallPrice",vecBinTreeUSCallPrice},{"BinTree US PutPrice",vecBinTreeUSPutPrice},
        {"BTerror1",vecBinTreeError1},{"BTerror2",vecBinTreeError2},{"BTerror3",vecBinTreeError3},{"BTerror4",vecBinTreeError4},{"BTerror5",vecBinTreeError5},{"BTerror6",vecBinTreeError6},{"BTerror7",vecBinTreeError7},{"BTerror8",vecBinTreeError8},{"BTerror9",vecBinTreeError9},{"BTerror10",vecBinTreeError10}};
    MyParameter1.WriteValue("BinTreeResult.csv",OutputDataset);

    // Binomial Tree with Barrier Using Specified Values
    cout << "---- Binomial Tree with Barrier (specified values) ---- " << endl;
    cout << "< EU--Call  Put    < US --Call  Put" << endl;

    r_int = 0.01;
    numSteps=200;   
    SpotPrice=10;
    Maturity=0.3;
    Sigma=0.20;
    K1=10;
    H=11;

    BinTree<double> PriceTreeBarr(numSteps,SpotPrice,r_int, 0, Sigma,Maturity);  // Initialize for a Binomial tree object 'PriceTreeBarr'
    PriceTreeBarr.BuildTree();

    TreeEuropean europeanOption3(K1,call);

    double PriceBinTreeUO_EU =PriceTreeBarr.GetPriceBarrier_UO(europeanOption3,H);

    cout << "Bin Tree Price UO for European Call: " <<  PriceBinTreeUO_EU << endl;
    cout << "Model Price UO: "<< PriceTreeBarr.CalcModelPriceBarrier(K1,H,UO) << endl;

    BinTree<double> PriceTree2(numSteps,SpotPrice,r_int, 0, Sigma,Maturity);  // Initialize for a Binomial tree object 'PriceTree'
    PriceTree2.BuildTree();

    cout << "Bin Tree Price UI for European Call: " <<  PriceTree2.GetPrice(europeanOption3)-PriceBinTreeUO_EU << endl;
    cout << "Model Price UI "<< PriceTreeBarr.CalcModelPriceBarrier(K1,H,UI) << endl;

    cout << "---------" << endl;
    TreeAmerican americanOption3(K1,put);

    double PriceBinTreeUO_US =PriceTreeBarr.GetPriceBarrier_UO(americanOption3,H);
    cout << "Bin Tree Price UO for American Put: " << PriceBinTreeUO_US << endl;
    cout << "Bin Tree Price for American Put: " << PriceTree2.GetPrice(americanOption3) << endl;
    cout << "Bin Tree Price UI for American Put: " << PriceTree2.GetPrice(americanOption3)-PriceBinTreeUO_US << endl;

    BS_Put myPut3(SpotPrice,K1,r_int,0,Maturity);    // Setup the Option parameters
    cout << "BS Put Option Price "<< myPut3(Sigma); // Calculate Option Price using B. Schole Model

    return 0;
}
