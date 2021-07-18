#include <cmath>

using std::endl;
using std::cout;

template<class T>
double Secant(double Target,double x0,double step,double Tolerance,T myFunction)
{
    double y_slope;
    double x1;
    double y0;
    double y1;

    x1=x0;

    do
    {
        x0=x1;
        y_slope=(myFunction(x0)-myFunction(x0-step))/step;
        y0=myFunction(x0);
        x1=x0 - (y0-Target)/y_slope;
        y1=myFunction(x1);

        //cout << "Trying Secant ..." << x1 << "  " << y1 << "  " << y_slope << endl;
    } while ((fabs(y1-Target)>Tolerance));

    return x1;
}