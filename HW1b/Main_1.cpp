#include "Functions.h"
#include "Numeric.h"
#include "Compute.h"
#include <iostream>
#include <cmath>
#include <cstring>

using std::cout;
using std::endl;
using std::string;

#define PI 4*atan(1)

int main()
{
    double diff_error;
    double a;
    double result;
    double prev_result;
    double tolerance=1e-4;
    int n_tol;

    double n,m;
    SampleFunction1 f;  // instantiate the function f(x)
    SampleFunction2D1 g1; // instantiate 2 dimension g1(x,y) and g2(x,y)
    SampleFunction2D2 g2;
    double g1_target=4.5; // Expected Values
    double g2_target=(exp(3)-1)*(exp(2)-1)/2;

    string Text;

    cout << "value of PI is " << PI << endl;

    for (int p=0;p<2;p++)  // repeat for two Methods
    {
        if (p==0) Text= "Trapezoid Method ";
        else Text= "Simpson Method ";
        cout << Text << endl;
        cout << "result   a   n    Trunc Error" << endl;

        for (int i=2;i<=4;i++) // Calculate truncation error & integration est for combination a & n
        {
            for (int j=2;j<=4;j++)
            {
                a = pow(10.0,i);
                n = pow(10.0,j);
                if (p==0){
                    diff_error = TruncError_Trapezoid (a,n,f,PI,result);}
                else {
                    diff_error = TruncError_Simpson(a,n,f,PI,result);
                }

                cout << a << " " << n  << "  " << result << "  " << diff_error << endl;
            }
        }
        cout << "------" << endl;
    }

    a=5000;   // Suppose a case with a=5000
    prev_result=0;
    for (int p=0;p<2;p++){ // Calculate truncation error & integration est until mean |Ik-Ik-1| < E
        for (int n=100;n<=100000;n++)
        {
            if (p==0) {
                diff_error = TruncError_Trapezoid(a,n,f,PI,result);
                Text ="Trapezoid Method";
            }
            else {
                diff_error = TruncError_Simpson(a,n,f,PI,result);
                Text = "Simpson Method";
            }
            if (fabs(result-prev_result) <= tolerance) {
                n_tol=n;
                break;
            }
            prev_result=result;
        }
    cout << "The n when satisfy tolerance " << tolerance << " is " << n_tol << " under " << Text << endl;
    cout << endl;
    }
	
    // Perform for double integral
    cout << "Value of g1 & g2 target " << g1_target << " and " << g2_target << endl;
    for (int p=0;p<2;p++)  // repeat for two function g1 & g2
    {
        if (p==0) Text= "Function g1=x*y^2";
        else Text= "Function g2=exp(2*x+y)";
        cout << Text << endl;
        cout << "result   n   m    Trunc Error" << endl;

        for (int j=2;j<=3;j++) // Calculate truncation error & double integral estimation
        {
            for (int i=2;i<=3;i++)
            {
                n = pow(10.0,i);
                m = pow(10.0,j);
                if (p==0) {
                    diff_error = TruncError_Double_Integral(0,1,n,0,3,m,g1,g1_target,result);
                } else {
                    diff_error = TruncError_Double_Integral(0,1,n,0,3,m,g2,g2_target,result);
                }
                cout << n << " " << m  << "  " << result << "  " << diff_error << endl;
            }
        }
    }

    return 0;
}
