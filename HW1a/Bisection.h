#include <cmath>

using std::endl;
using std::cout;

template<class T>
double Bisection(double Target,double a,double b,double Tolerance,T myFunction)
{
    double mid=0.5*(a+b);
    double y_a=myFunction(a);
    double y_mid=myFunction(mid);

    double f_a=y_a-Target;
    double f_mid=y_mid-Target;

    // Do while tolerance still large
    do
    {
        // Adjust b if root falls between (a,(a+b)/2)
        if ((f_a*f_mid)<0){
            b=(a+b)/2;
        }
        // Adjust a if root falls outside (a,(a+b)/2)
        if ((f_a*f_mid)>0){
            a=(a+b)/2;
        }
        mid=0.5*(a+b);
        y_a=myFunction(a);
        y_mid=myFunction(mid);
        f_a=y_a-Target;
        f_mid=y_mid-Target;

    } while ((fabs(f_mid)>Tolerance));

    return mid;
}