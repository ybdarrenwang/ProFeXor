#ifndef MYMATH_H
#define MYMATH_H

#include "global.h"

using namespace std;

// change array into vector
template <class X>
vector<X> array2Vec(X* in, int size)
{
	vector<X> out;
	int i;

	for(i=0;i<size;i++)
		out.push_back(in[i]);

	return out;
}

void Vector_Axe(int, vector<double>&);
void Array_Axe(int, double*);
vector<double> Vector_Zeros(int);

vector<double> subVec(vector<double>, int, int);
double Vector_InnerProd(vector<double>&, vector<double>&);
void Vector_Sum(vector<double>&, vector<double>&);
void Vector_Mul(vector<double>&, double);
void Vector_log(vector<double>&);
void Vector_WindowAvg(vector<double>&, int, vector<double>&);

double getMean(double*, const int);
double getMean(vector<double>&);
double getSlope(double*, double*, int);
double getSlope(double*, int);
double getSlope(vector<double>&);
double getSlope(vector<double>&, vector<double>&);
double getVar(double*, int, double, bool);
double getVar(double*, int, bool);
double getVar(double*, int);
double getVar(vector<double>&);
double getSD(double*, const int);
double getSD(vector<double>&);
double getCov(vector<double>&, vector<double>&, double, double);
double getCov(vector<double>&, vector<double>&);

void LinearRegression(vector<double>&, vector<double>&, double*);
void LinearRegression(double*, double*, const int, double*);
void LinearRegression(vector<double>&, double*);
void LinearRegression(double*, const int, double*);
double SquareError(vector<double>&, vector<double>&, double*);
double SquareError(double*, double*, const int, double*);
double SquareError(vector<double>&, vector<double>&);
double SquareError(vector<double>&, double*);
double SquareError(double*, const int, double*);
double SquareError(vector<double>&);

void LinearInterpolate(vector<double>&, vector<double>&, int);

void getMinMax(double*, double&, double&, int);
void getMinMax_discontinue(double*, double&, double&, int);
double getMax(double*, const int);
double getMax(vector<double>&);

double avgMaxPitch(double*, int, int);
double avgMinPitch(double*, int, int);
double avgPch(vector<double>);

/**
* Discrete Legendre Coefficient\n
* Note: L0 is simply syllable-level pitch mean!
*/
void Legendre_coeff(vector<double>& pch, double* a);

#endif
