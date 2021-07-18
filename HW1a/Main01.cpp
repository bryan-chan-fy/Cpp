#include "Parameters.h"
#include "BSModel01.h"
#include "Bisection.h"
#include "Secant.h"
#include "PutCallParity.h"

#include <string>
#include <cmath>
#include <chrono>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using ms=std::chrono::duration<double,std::milli>;

typedef vector<pair<string,vector<double>>> VectorPair;

int main()
{
    double SpotEq1[2];  // j=0 Day 1; j=1 Day 2
    double SpotEq2[2];  // j=0 Day 1; j=1 Day 2
    double Expiry;
    double CallPrice;
    double PutPrice;
    double CallPriceD2;
    double PutPriceD2;
    double StrikeEq1;
    double StrikeEq2;
    double SigmaCall;
    double SigmaPut;
    double ExpectedPrice;
    double Tolerance;
    double Sigma2;
    int i;
    double r[2];
    string filenameEq1;
    string filenameEq2;
    double Price_CallOption_Eq1[9],Price_PutOption_Eq1[9];
    double Price_CallOption_Eq2[9],Price_PutOption_Eq2[9];
    double Price_CallParity_Eq1[9],Price_PutParity_Eq1[9];
    double Price_CallParity_Eq2[9],Price_PutParity_Eq2[9];
    double Price_CallOptionD2_Eq1[9], Price_PutOptionD2_Eq1[9];  // store extracted Day 2 Option Price
    double Price_CallOptionD2_Eq2[9], Price_PutOptionD2_Eq2[9];
    double Price_CallOptionD2Calc_Eq1[9], Price_PutOptionD2Calc_Eq1[9]; // store calculated Day 2 Option Price
    double Price_CallOptionD2Calc_Eq2[9], Price_PutOptionD2Calc_Eq2[9];

    // TbData_xxx [j][i]=odd j=day1 ; even j=day2 ; i=0 contract name; j=1 Implied Volatility
    string TbData_Eq1_Call[9][2]; string TbData_Eq1_Put[9][2];
    string TbData_Eq2_Call[9][2]; string TbData_Eq2_Put[9][2];
    double TbAvg[9][2];
    double Delta_Call_Eq1[9], Delta_Call_Eq2[9];
    double Gamma_Call_Eq1[9], Gamma_Call_Eq2[9];
    double Vega_Call_Eq1[9], Vega_Call_Eq2[9];

    vector<double> vecT;
    vector<double> vecStrikeEq1;
    vector<double> vecStrikeEq2;
    vector<double> vecAvgSigmaEq1;
    vector<double> vecAvgSigmaEq2;
    VectorPair OutputDataset;

    // Contract names
    TbData_Eq1_Call[0][0]="AMZN201120C03000000.Close"; TbData_Eq1_Put[0][0]="AMZN201120P03000000.Close";
    TbData_Eq1_Call[1][0]="AMZN201120C03200000.Close"; TbData_Eq1_Put[1][0]="AMZN201120P03200000.Close";
    TbData_Eq1_Call[2][0]="AMZN201120C03500000.Close"; TbData_Eq1_Put[2][0]="AMZN201120P03500000.Close";
    TbData_Eq1_Call[3][0]="AMZN201218C03000000.Close"; TbData_Eq1_Put[3][0]="AMZN201218P03000000.Close";
    TbData_Eq1_Call[4][0]="AMZN201218C03200000.Close"; TbData_Eq1_Put[4][0]="AMZN201218P03200000.Close";
    TbData_Eq1_Call[5][0]="AMZN201218C03500000.Close"; TbData_Eq1_Put[5][0]="AMZN201218P03500000.Close";
    TbData_Eq1_Call[6][0]="AMZN210115C03000000.Close"; TbData_Eq1_Put[6][0]="AMZN210115P03000000.Close";
    TbData_Eq1_Call[7][0]="AMZN210115C03200000.Close"; TbData_Eq1_Put[7][0]="AMZN210115P03200000.Close";
    TbData_Eq1_Call[8][0]="AMZN210115C03500000.Close"; TbData_Eq1_Put[8][0]="AMZN210115P03500000.Close";

    TbData_Eq2_Call[0][0]="SPY201120C00320000.Close"; TbData_Eq2_Put[0][0]="SPY201120P00320000.Close";
    TbData_Eq2_Call[1][0]="SPY201120C00340000.Close"; TbData_Eq2_Put[1][0]="SPY201120P00340000.Close";
    TbData_Eq2_Call[2][0]="SPY201120C00360000.Close"; TbData_Eq2_Put[2][0]="SPY201120P00360000.Close";
    TbData_Eq2_Call[3][0]="SPY201218C00320000.Close"; TbData_Eq2_Put[3][0]="SPY201218P00320000.Close";
    TbData_Eq2_Call[4][0]="SPY201218C00340000.Close"; TbData_Eq2_Put[4][0]="SPY201218P00340000.Close";
    TbData_Eq2_Call[5][0]="SPY201218C00360000.Close"; TbData_Eq2_Put[5][0]="SPY201218P00360000.Close";
    TbData_Eq2_Call[6][0]="SPY210115C00320000.Close"; TbData_Eq2_Put[6][0]="SPY210115P00320000.Close";
    TbData_Eq2_Call[7][0]="SPY210115C00340000.Close"; TbData_Eq2_Put[7][0]="SPY210115P00340000.Close";
    TbData_Eq2_Call[8][0]="SPY210115C00360000.Close"; TbData_Eq2_Put[8][0]="SPY210115P00360000.Close";

    r[0]=0.09/100;
    r[1]=0.09/100;

    Parameters MyParameter1;
    // Extract Spot prices (EOD price)
    for (int j=0;j<2;j++){
        SpotEq1[j]=MyParameter1.GetValue("yahoo_fdata1_norow.csv","AMZN.Close",j);
        cout << SpotEq1[j] << " Extracted Spot Price Equity 1 for Day " << j+1 << endl;
        SpotEq2[j]=MyParameter1.GetValue("yahoo_fdata1_norow.csv","SPY.Close",j);
        cout << SpotEq2[j] << " Extracted Spot Price Equity 1 for Day " << j+1 << endl;

    }

    i=0;
    while (i<9){
        if (i<3){
            filenameEq1="yahoo_fdata2_norow.csv";
            filenameEq2="yahoo_fdata3_norow.csv";
            Expiry=50.0/252;        // 0. decimal is necessary
        }
        if (i>=3 && i<6){
            filenameEq1="yahoo_fdata2b_norow.csv";
            filenameEq2="yahoo_fdata3b_norow.csv";
            Expiry=70.0/252;        // 0. decimal is necessary
        }
        if (i>=6 && i<9){
            filenameEq1="yahoo_fdata2c_norow.csv";
            filenameEq2="yahoo_fdata3c_norow.csv";
            Expiry=90.0/252;        // 0. decimal is necessary
        }
        if (i%3==0){
            StrikeEq1=3000.0;
            StrikeEq2=320.0;
        }

        if (i%3==1){
            StrikeEq1=3200.0;
            StrikeEq2=340.0;
        }
        if (i%3==2){
            StrikeEq1=3500.0;
            StrikeEq2=360.0;
        }

        CallPrice=MyParameter1.GetValue(filenameEq1,TbData_Eq1_Call[i][0],0);
        PutPrice=MyParameter1.GetValue(filenameEq1,TbData_Eq1_Put[i][0],0);
        Price_CallOption_Eq1[i]=CallPrice;
        Price_PutOption_Eq1[i]=PutPrice;
        cout << CallPrice << " Extracted Call Price for Eq1 " << "  ";
        cout << PutPrice << " Extracted Put Price for Eq1" <<endl;

        CallPriceD2=MyParameter1.GetValue(filenameEq1,TbData_Eq1_Call[i][0],1);  // Day 2 Option Price
        PutPriceD2=MyParameter1.GetValue(filenameEq1,TbData_Eq1_Put[i][0],1);
        Price_CallOptionD2_Eq1[i]=CallPriceD2;
        Price_PutOptionD2_Eq1[i]=PutPriceD2;

        // Sep 14 2020 - Nov 20 2020= 50 business days
        // Sep 14 2020 - Dec 18 2020= 70 business days
        // Sep 14 2020 - Jan 15 2021= 90 business days

        Tolerance=1e-6;

        BS_Call myCall1(SpotEq1[0],StrikeEq1,r[0],Expiry);    // Setup the Option parameters
        BS_Put myPut1(SpotEq1[0],StrikeEq1,r[0],Expiry);

        const auto before1 = std::chrono::system_clock::now();

        SigmaCall = Bisection(CallPrice,0.01,0.9,Tolerance,myCall1); // Implied volatility for AMZN
        SigmaPut = Bisection(PutPrice,0.01,2.0,Tolerance,myPut1);

        const ms duration1 =std::chrono::system_clock::now()-before1;
        cout << "It took " << float(duration1.count()) << "ms" << endl;

        TbData_Eq1_Call[i][1]=std::to_string(SigmaCall);
        TbData_Eq1_Put[i][1]=std::to_string(SigmaPut);

        cout << SigmaCall << " Calculated Call Sigma Bisec for Eq1   ";
        cout << SigmaPut<< " Calculated Put Sigma Bisec for Eq1" << endl;

        const auto before2 = std::chrono::system_clock::now();

        SigmaCall = Secant(CallPrice,1.0,0.01,Tolerance,myCall1);  // Implied volatility for AMZN
        SigmaPut = Secant(PutPrice,1.0,0.01,Tolerance,myPut1);

        const ms duration2 =std::chrono::system_clock::now()-before2;
        cout << "It took " << float(duration2.count()) << "ms" << endl;

        cout << SigmaCall << " ------Calculated Call Sigma Secant for Eq1   ";
        cout << SigmaPut << " ------Calculated Put Sigma Secant for Eq1" << endl;

        Delta_Call_Eq1[i]= myCall1.GetDelta(SigmaCall);	 // Calculate Greeks for AMZN
        Gamma_Call_Eq1[i]= myCall1.GetGamma(SigmaCall);
        Vega_Call_Eq1[i]= myCall1.GetVega(SigmaCall);


        CallPrice=MyParameter1.GetValue(filenameEq2,TbData_Eq2_Call[i][0],0);
        PutPrice=MyParameter1.GetValue(filenameEq2,TbData_Eq2_Put[i][0],0);
        Price_CallOption_Eq2[i]=CallPrice;
        Price_PutOption_Eq2[i]=PutPrice;
        cout << CallPrice << " Extracted Call Price for Eq2" << "  ";
        cout << PutPrice << " Extracted Put Price for Eq2" <<endl;

        CallPriceD2=MyParameter1.GetValue(filenameEq2,TbData_Eq2_Call[i][0],1);  // Day 2 Option Price
        PutPriceD2=MyParameter1.GetValue(filenameEq2,TbData_Eq2_Put[i][0],1);
        Price_CallOptionD2_Eq2[i]=CallPriceD2;
        Price_PutOptionD2_Eq2[i]=PutPriceD2;

        BS_Call myCall2(SpotEq2[0],StrikeEq2,r[0],Expiry);    // Setup the Option parameters
        BS_Put myPut2(SpotEq2[0],StrikeEq2,r[0],Expiry);

        SigmaCall = Bisection(CallPrice,0.01,0.9,Tolerance,myCall2);  // Implied volatility for SPY
        SigmaPut = Bisection(PutPrice,0.01,2.0,Tolerance,myPut2);
        cout << SigmaCall << " Calculated Call Sigma Bisec for Eq2   ";
        cout << SigmaPut<< " Calculated Put Sigma Bisec for Eq2" << endl;

        SigmaCall = Secant(CallPrice,1.0,0.01,Tolerance,myCall2);
        SigmaPut = Secant(PutPrice,1.0,0.01,Tolerance,myPut2);
        cout << SigmaCall << " -----------Calculated Call Sigma Secant for Eq2   ";
        cout << SigmaPut << " -----------Calculated Put Sigma Secant for Eq2" << endl;

        TbData_Eq2_Call[i][1]=std::to_string(SigmaCall);
        TbData_Eq2_Put[i][1]=std::to_string(SigmaPut);

        Delta_Call_Eq2[i]= myCall2.GetDelta(SigmaCall);	 // Calculate Greeks for SPY
        Gamma_Call_Eq2[i]= myCall2.GetGamma(SigmaCall);
        Vega_Call_Eq2[i]= myCall2.GetVega(SigmaCall);

        i++;
    }

    cout << "--- Display Implied Volatilities----" << endl;

    // Display AMZN Call & Put Implied Volatilities
    for (int i=0;i<9;i++){
        cout << TbData_Eq1_Call[i][1] << "   ";
        cout << TbData_Eq1_Put[i][1] << endl;
        // Call & Put average
        TbAvg[i][0] = (std::stod(TbData_Eq1_Call[i][1])+std::stod(TbData_Eq1_Put[i][1]))/2;
    }
    cout << "------------" << endl;

    // Display SPY Call & Put Implied Volatilities
    for (int i=0;i<9;i++){
        cout << TbData_Eq2_Call[i][1] << "   ";
        cout << TbData_Eq2_Put[i][1] << endl;
        // Call & Put average
        TbAvg[i][1]=(std::stod(TbData_Eq2_Call[i][1])+std::stod(TbData_Eq2_Put[i][1]))/2;
    }

    // Calculate Put Call Parity
    for (int i=0;i<9;i++){
        if (i<3){
            Expiry=50.0/252;
        }
        if (i>=3 && i<6){
            Expiry=70.0/252;
        }
        if (i>=6 && i<9){
            Expiry=90.0/252;
        }
        if (i%3==0){
            StrikeEq1=3000.0;
            StrikeEq2=320.0;
        }
        if (i%3==1){
            StrikeEq1=3200.0;
            StrikeEq2=340.0;
        }
        if (i%3==2){
            StrikeEq1=3500.0;
            StrikeEq2=360.0;
        }
            Price_CallParity_Eq1[i]=PutCallParity(Price_PutOption_Eq1[i],SpotEq1[0],r[0],Expiry,StrikeEq1,true);
            Price_PutParity_Eq1[i]=PutCallParity(Price_CallOption_Eq1[i],SpotEq1[0],r[0],Expiry,StrikeEq1,false);
            Price_CallParity_Eq2[i]=PutCallParity(Price_PutOption_Eq2[i],SpotEq2[0],r[0],Expiry,StrikeEq2,true);
            Price_PutParity_Eq2[i]=PutCallParity(Price_CallOption_Eq2[i],SpotEq2[0],r[0],Expiry,StrikeEq2,false);

    }

    cout << "----Display Put Call Parity -----" << endl;

    // Display Put Call Parity
    for (int i=0;i<9;i++) {
    cout << Price_CallOption_Eq1[i] << " " << Price_CallParity_Eq1[i] << "   " << Price_PutOption_Eq1[i] << " " << Price_PutParity_Eq1[i] <<endl;
    }
    cout << "----" << endl;
    for (int i=0;i<9;i++) {
    cout << Price_CallOption_Eq2[i] << " " << Price_CallParity_Eq2[i] << "   " << Price_PutOption_Eq2[i] << " " << Price_PutParity_Eq2[i] <<endl;
    }

    cout << "-----Display Delta, Gamma, Vega -----" <<endl;
    for (int i=0;i<9;i++) {
    cout << Delta_Call_Eq1[i] << "   " << Gamma_Call_Eq1[i] << "   " << Vega_Call_Eq1[i] << endl;
    }
    cout << "-----" << endl;
        for (int i=0;i<9;i++) {
    cout << Delta_Call_Eq2[i] << "   " << Gamma_Call_Eq2[i] << "   " << Vega_Call_Eq2[i] << endl;
    }

    // BS_Call myCall3(337.49,320.0,0.09/100,50.0/252);  // checking opening Spot implied volatility
    // SigmaCall = Secant(26.01,1.0,0.01,1e-6,myCall3);
    // cout << SigmaCall << "  <- endl";

    // Compute Day 2 Option Prices using Day 1 implied volatility & then Display
    for (int i=0;i<9;i++) {
        if (i<3)
            Expiry=50.0/252;
        if (i>=3 && i<6)
            Expiry=70.0/252;
        if (i>=6 && i<9)
            Expiry=90.0/252;
        if (i%3==0){
            StrikeEq1=3000.0; StrikeEq2=320.0;
        }
        if (i%3==1){
            StrikeEq1=3200.0; StrikeEq2=340.0;
        }
        if (i%3==2){
            StrikeEq1=3500.0; StrikeEq2=360.0;
        }
        BS_Call myCall1(SpotEq1[1],StrikeEq1,r[1],Expiry);    // Setup the Option parameters
        BS_Put myPut1(SpotEq1[1],StrikeEq1,r[1],Expiry);
        Price_CallOptionD2Calc_Eq1[i]=myCall1(std::stod(TbData_Eq1_Call[i][1] )); // Calculate Option Prices using imp. vol.
        Price_PutOptionD2Calc_Eq1[i]=myPut1(std::stod(TbData_Eq1_Put[i][1]));

        BS_Call myCall2(SpotEq2[1],StrikeEq2,r[1],Expiry);    // Setup the Option parameters
        BS_Put myPut2(SpotEq2[1],StrikeEq2,r[1],Expiry);
        Price_CallOptionD2Calc_Eq2[i]=myCall2(std::stod(TbData_Eq2_Call[i][1] )); // Calculate Option Prices using imp. vol.
        Price_PutOptionD2Calc_Eq2[i]=myPut2(std::stod(TbData_Eq2_Put[i][1]));
    }
    cout << "-----Display Day 2 Calculated Call & Put Option Prices -----" << endl;
    for (int i=0;i<9;i++) {
        cout << Price_CallOptionD2Calc_Eq1[i] << "   "<< Price_CallOptionD2_Eq1[i] << "        " << Price_PutOptionD2Calc_Eq1[i] << "   "<< Price_PutOptionD2_Eq1[i] << endl;
    }
    cout << "-------" << endl;
    for (int i=0;i<9;i++) {
        cout << Price_CallOptionD2Calc_Eq2[i] << "   "<< Price_CallOptionD2_Eq2[i] << "        " << Price_PutOptionD2Calc_Eq2[i] << "   "<< Price_PutOptionD2_Eq2[i] << endl;
    }

    // Convert average volatilities to vector to write to csv file for R plotting
    for (int i=0;i<9;i++) {
        if (i<3)
            Expiry=50.0/252;
        if (i>=3 && i<6)
            Expiry=70.0/252;
        if (i>=6 && i<9)
            Expiry=90.0/252;
        if (i%3==0){
            StrikeEq1=3000.0; StrikeEq2=320.0;
        }
        if (i%3==1){
            StrikeEq1=3200.0; StrikeEq2=340.0;
        }
        if (i%3==2){
            StrikeEq1=3500.0; StrikeEq2=360.0;
        }
        //VectT.at(colIndex).second.push_back(val);
        vecT.push_back(Expiry);
        vecStrikeEq1.push_back(StrikeEq1);
        vecStrikeEq2.push_back(StrikeEq2);
        vecAvgSigmaEq1.push_back(TbAvg[i][0]);
        vecAvgSigmaEq2.push_back(TbAvg[i][1]);
    }
    OutputDataset = {{"Expiry",vecT},{"Strike Eq1",vecStrikeEq1},{"Strike Eq2",vecStrikeEq2},{"Avg Imp Vol Eq1",vecAvgSigmaEq1},{"Avg Imp Vol Eq2",vecAvgSigmaEq2}};
    MyParameter1.WriteValue("VolResult.csv",OutputDataset);

    return 0;
}
