#ifndef COMPUTE_H
#define COMPUTE_H

#include "Numeric.h"
#include "Functions.h"

enum MethodType {Trapezoid,Simpson,Double};

double Compute(const Numeric& Area, MethodType Method);

double TruncError_Trapezoid(double a, int n, MathFunction& f, double Target, double& int_result);

double TruncError_Simpson(double a, int n, MathFunction& f, double Target, double& int_result);

double TruncError_Double_Integral(double a, double b, int n, double c, double d, int m, MathFunction& f, double Target, double& int_result);

#endif // COMPUTE_H