#include "mymath.h"

using namespace std;

// ==================================================================
// Vector creation
// ==================================================================
// create vector with the i-th element = i
void Vector_Axe(int size, vector<double>& axe)
{
	int i;

	if(!axe.empty()) axe.clear();

	for(i=0;i<size;i++)
		axe.push_back(i);
}

void Array_Axe(int size, double* axe)
{
	axe = new double[size];
	int i;

	for(i=0;i<size;i++)
		axe[i] = i;
}

// create zero vector
vector<double> Vector_Zeros(int size)
{
	vector<double> z;
	int i;
	for(i=0;i<size;i++)
		z.push_back(0);
	return z;
}

// create sequence vector given the first and last elements
vector<double> Vector_Series(int bg, int end)
{
	vector<double> series;
	int i;
	for(i=bg;i<=end;i++)
		series.push_back(i);
	return series;
}

// return the sub-vector of input iv
// which is [iv(bg),...,iv(end)]
vector<double> subVec(vector<double> iv, int bg, int end)
{
	vector<double> v;
	int i;
	for( i=bg; i<=end; i++)
		v.push_back(iv[i]);
	
	return v;
}

// ==================================================================
// Vector manipulation
// ==================================================================
double Vector_InnerProd(vector<double>& a, vector<double>& b)
{
	double answer = 0;

	if (a.size() != b.size())
	{
		cerr<<"Error: vector size does not match"<<endl;
		exit(1);
	}

	for (int i=0; i<(int)a.size(); i++)
		answer += a[i]*b[i];

	return answer;
}

void Vector_Sum(vector<double>& a, vector<double>& b, vector<double>& answer)
{
	if (!answer.empty()) answer.clear();

	if (a.size() != b.size())
	{
		cerr<<"Error: vector size does not match"<<endl;
		exit(1);
	}

	for (int i=0; i<(int)a.size(); i++)
		answer.push_back(a[i]+b[i]);
}

void Vector_Sum(vector<double>& a, vector<double>& b)
{
	if (a.size() != b.size())
	{
		cerr<<"Error: vector size does not match"<<endl;
		exit(1);
	}

	for (int i=0; i<(int)a.size(); i++)
		a[i]+=b[i];
}

void Vector_Mul(vector<double>& a, double b)
{
	for (int i=0; i<(int)a.size(); i++)
		a[i]*=b;
}

void Vector_log(vector<double>& a)
{
	for (int i=0; i<(int)a.size(); i++)
	{
		if (a[i] > 0)
			a[i] = log(a[i]);
	}
}

void Vector_WindowAvg(vector<double>& a, int window_size, vector<double>& average)
{
	int i, j, win_start, win_end;
	double tmp_avg;
	int half_win = (int)((double)(window_size-1)/2);
	if (!average.empty()) average.clear();

	for (i=0; i<(int)a.size(); i++)
	{
		if (i<half_win) win_start = 0;
		else win_start = i-half_win;

		if (i>=(int)a.size()-half_win) win_end = a.size()-1;
		else win_end = i+half_win;

		tmp_avg = 0;
		for (j=win_start; j<=win_end; j++)
			tmp_avg += a[j];
		average.push_back(tmp_avg/(win_end-win_start+1));
	}
}

// ==================================================================
// Calculate mean, slope and variance
// ==================================================================
double getMean(double* data, const int N)
{
	if (N == 0) return 0;
	else if (N == 1) return data[0];
	else
	{
		int k;
		double sum=0;

		for(k=0; k<N; k++)
			sum += data[k];

		return sum/N;
	}
}

double getMean(vector<double>& data)
{
	return getMean(&data[0], data.size());
}

double getSlope(double* x, double* y, int N)
{
	if (N<=1) return 0;
	else
	{
		double XY = 0;
		double XX = 0;
		double x_mean = getMean(x, N);
		double y_mean = getMean(y, N);
		int i;

		for(i=0; i<N; i++)
		{
			XY += (x[i]-x_mean)*(y[i]-y_mean);
			XX += (x[i]-x_mean)*(x[i]-x_mean);
		}

		if (XX == 0)
			return 1e38;
		else
			return XY/XX;
	}
}

double getSlope(double* y, int N)
{
	double* axe;
	Array_Axe(N, axe);
	double slope = getSlope(axe, y, N);
	delete [] axe;
	return slope;
}

double getSlope(vector<double>& x, vector<double>& y)
{
	if (x.size() != y.size())
	{
		cerr<<"Error: size of x and y for slope calculation is different"<<endl;
		exit(1);
	}
	if (x.size() <= 1) return 0;
	else
	{
		double XY = 0;
		double XX = 0;
		double x_mean = getMean(x);
		double y_mean = getMean(y);
		int i;

		for(i=0; i<(int)x.size(); i++)
		{
			XY += (x[i]-x_mean)*(y[i]-y_mean);
			XX += (x[i]-x_mean)*(x[i]-x_mean);
		}

		if (XX == 0)
			return 1e38;
		else
			return XY/XX;
	}
}

double getSlope(vector<double>& y)
{
	vector<double> x;
	Vector_Axe(y.size(), x);
	return getSlope(x, y);
}

// if there's discontinuity in the data, ex. pitch contour,
// you can skip "0" by assigning discontinue = true.
double getVar(double* data, int N, double mean, bool discontinue)
{
	if (N<=1) return 0;
	else
	{
		double sum = 0;
		int k;
		int counter = 0;

		for(k=0; k<N; k++)
		{
			if (discontinue == false || data[k] > 0)
			{
				sum += (data[k]-mean)*(data[k]-mean);
				counter++;
			}
		}

		return sum/counter;
	}
}

double getVar(double* data, int N, bool discontinue)
{
	return getVar(data, N, getMean(data, N), discontinue);
}

double getVar(double* data, int N)
{
	return getVar(data, N, getMean(data, N), false);
}

double getVar(vector<double>& data)
{
	return getVar(&(data[0]), data.size());
}

double getSD(double* data, int N)
{
	return sqrt(getVar(data, N));
}

double getSD(vector<double>& data)
{
	return sqrt(getVar(&(data[0]), data.size()));
}

double getCov(vector<double>& x, vector<double>& y, double x_mean, double y_mean)
{
	int i;
	vector<double> tmp;

	if (x.size() != y.size())
	{
		cerr<<"Error: sizes of vectors for covariance calculation unequal"<<endl;
		exit(1);
	}

	if (x_mean != 0)
	{
		for (i=0; i<(int)x.size(); i++)
			x[i] -= x_mean;
	}
	if (y_mean != 0)
	{
		for (i=0; i<(int)y.size(); i++)
			y[i] -= y_mean;
	}

	for (i=0; i<(int)x.size(); i++)
		tmp.push_back(x[i]*y[i]);

	return getMean(tmp);
}

double getCov(vector<double>& x, vector<double>& y)
{
	return getCov(x, y, getMean(x), getMean(y));
}

double getMax(double* x, const int N)
{
	double max = x[0];
	for(int i=1; i<N; i++)
	{
		if (x[i] > max)
			max = x[i];
	}
	return max;
}

double getMax(vector<double>& x)
{
	double max = x[0];
	for(int i=1; i<x.size(); i++)
	{
		if (x[i] > max)
			max = x[i];
	}
	return max;
}

// ==================================================================
// Perform linear regression; Calculate square error
// ==================================================================
void LinearRegression(vector<double>& x, vector<double>& y, double* coeff)
{
	if (x.size() != y.size())
	{
		cerr<<"Error(LinearRegression): sizes of vectors x and y are different"<<endl;
		exit(1);
	}
	if (coeff == 0)
		coeff = new double[2];

	if (x.size() == 1)
	{
		coeff[0] = 0;
		coeff[1] = y[0];
	}
	else
	{
		double XY = 0;
		double XX = 0;
		double x_mean = getMean(x);
		double y_mean = getMean(y);

		for(int i=0; i<(int)x.size(); i++)
		{
			XY += (x[i]-x_mean)*(y[i]-y_mean);
			XX += (x[i]-x_mean)*(x[i]-x_mean);
		}

		if (XX == 0)
		{
			coeff[0] = 1e38;
			coeff[1] = -1*1e38;
		}
		else
		{
			coeff[0] = XY/XX;
			coeff[1] = y_mean - coeff[0]*x_mean;
		}
	}
}

void LinearRegression(double* x, double* y, const int size, double* coeff)
{
	if (coeff == 0)
		coeff = new double[2];

	if (size == 1)
	{
		coeff[0] = 0;
		coeff[1] = y[0];
	}
	else
	{
		double XY = 0;
		double XX = 0;
		double x_mean = getMean(x, size);
		double y_mean = getMean(y, size);

		for(int i=0; i<size; i++)
		{
			XY += (x[i]-x_mean)*(y[i]-y_mean);
			XX += (x[i]-x_mean)*(x[i]-x_mean);
		}

		if (XX == 0)
		{
			coeff[0] = 1e38;
			coeff[1] = -1*1e38;
		}
		else
		{
			coeff[0] = XY/XX;
			coeff[1] = y_mean - coeff[0]*x_mean;
		}
	}
}

void LinearRegression(vector<double>& y, double* coeff)
{
	vector<double> x;
	Vector_Axe(y.size(), x);
	LinearRegression(x, y, coeff);
}

void LinearRegression(double* y, const int size, double* coeff)
{
	double x[size];
	for (int i=0; i<size; i++) x[i] = i;
	LinearRegression(x, y, size, coeff);
}

double SquareError(vector<double>& x, vector<double>& y, double* coeff)
{
	if (x.size() != y.size())
	{
		cerr<<"Error: sizes of vectors x and y are different"<<endl;
		exit(1);
	}

	double error = 0;

	for (int i=0; i<(int)x.size(); i++)
		error += pow(y[i]-coeff[0]*x[i]-coeff[1], 2.0);

	return error;
}

double SquareError(double* x, double* y, const int size, double* coeff)
{
	double error = 0;

	for (int i=0; i<size; i++)
		error += pow(y[i]-coeff[0]*x[i]-coeff[1], 2.0);

	return error;
}

double SquareError(vector<double>& x, vector<double>& y)
{
	double* coeff = new double[2];
	LinearRegression(x, y, coeff);
	if (coeff[0] == 1e38)
	{
		delete [] coeff;
		return 1e38;
	}
	else
	{
		delete [] coeff;
		return SquareError(x, y, coeff);
	}
}

double SquareError(vector<double>& y, double* coeff)
{
	vector<double> x;
	Vector_Axe(y.size(), x);
	return SquareError(x, y, coeff);
}

double SquareError(double* y, const int size, double* coeff)
{
	double x[size];
	for (int i=0; i<size; i++) x[i] = i;
	return SquareError(x, y, size, coeff);
}

double SquareError(vector<double>& y)
{
	vector<double> x;
	Vector_Axe(y.size(), x);
	double* coeff = new double[2];
	LinearRegression(x, y, coeff);
	if (coeff[0] == 1e38)
	{
		delete [] coeff;
		return 1e38;
	}
	else
	{
		delete [] coeff;
		return SquareError(x, y, coeff);
	}
}

// ==================================================================
// Linearly interpolate contour into [dim] points.
// (The first and last points are preserved)
// ==================================================================
void LinearInterpolate(vector<double>& in_contour, vector<double>& out_contour, int dim)
{
	double step, knot = 0;
	int begin, end;

	if (!out_contour.empty()) out_contour.clear();

	if ((int)in_contour.size() == dim)
		out_contour = in_contour;
	else
	{
		if ((int)in_contour.size() > dim)
			step = (double)in_contour.size()/(dim-1);
		else
			step = (double)in_contour.size()/(dim+1);

		out_contour.push_back(in_contour[0]);
		for (int i=1; i<dim-1; i++)
		{
			knot+=step;
			begin = floor(knot);
			end = ceil(knot);

			if (begin == end) // knot is integer
				out_contour.push_back(in_contour[begin]);
			else
				out_contour.push_back(in_contour[begin]*(end-knot) + in_contour[end]*(knot-begin));
		}
		out_contour.push_back(in_contour[(int)in_contour.size()-1]);
	}
}

// ==================================================================
// get minima and maxima using divide-and-conqure algorithm
// ==================================================================
void getCompare(double* data, double& min, double& max, int n)
{
	if (n == 1)
		min = max = data[0];
	else
	{
		if (data[0] >= data[1])
		{
			max = data[0];
			min = data[1];
		}
		else
		{
			max = data[1];
			min = data[0];
		}
	}
}

void getMinMax(double* data, double& min, double& max, int n)
{
	if (n > 2)
	{
		double min_1, max_1, min_2, max_2;
		getMinMax(&(data[0]), min_1, max_1, (int)floor(n/2));
		getMinMax(&(data[(int)floor(n/2)]), min_2, max_2, (int)ceil(n/2));

		if (min_1 < min_2)
			min = min_1;
		else
			min = min_2;

		if (max_1 >= max_2)
			max = max_1;
		else
			max = max_2;
	}
	else
		getCompare(data, min, max, n);
}

// for dis-continuous pitch contour
void getMinMax_discontinue(double* data, double& min, double& max, int n)
{
	double th = log(40);

	// copy nonzero pitch
	vector<double> tmp_data;
	for (int i=0; i<n; i++)
	{
		while(data[i] < th)
		{
			i++;
		}

		tmp_data.push_back(data[i]);
	}
	getMinMax(&(tmp_data[0]), min, max, tmp_data.size());
}

// ==================================================================
// the average of p max. pitches
// ==================================================================
double avgMaxPitch(double* pitch, int Length, const int p)
{
	double buffer[p];
	int k,j,i;
	for(k=0;k < p; k++)
		buffer[k]=0;

	for(k=0; k < Length; k++)
	{
		//cout<<pitch[k]<<" ";
		j = 0;
		i = 0;
		while(buffer[j]<pitch[k] && j<p) // find how many values are beaten
		{
			j++;
		}
		while(i < j-1) // shift left
		{
			buffer[i] = buffer[i+1];
			i++;
		}
		if (j>0) // put the maxima at the rightmost
		{
			buffer[j-1] = pitch[k];
		}
	}
	//cout<<endl;	
	double average = 0;
	for(k=0;k < p; k++)
	{
		average += buffer[k];
	}
	return average/p;
}

// ==================================================================
// the average of p min. pitches
// ==================================================================
double avgMinPitch(double* pitch, int Length, const int p)
{
	double buffer[p];

	int k,j,i;
	for(k=0;k < p; k++)
		buffer[k]=1000;

	for(k=0; k < Length; k++)
	{
		//cout<<pitch[k]<<" ";
		j = 0;
		i = 0;
		while(buffer[j]>pitch[k]&&j<p)
		{
			j++;
		}
		while(i < j-1)
		{
			buffer[i] = buffer[i+1];
			i++;
		}
		if (j>0)
		{
			buffer[j-1] = pitch[k];
		}
	}
	//cout<<endl;
	double average = 0;
	for(k=0;k < p; k++){
		average += buffer[k];
	}
	return average/p;
}

// ==================================================================
// the average of frame pitch
// ==================================================================
double avgPch(vector<double> f0_data)
{
	int i;
	double sum = 0;
	int counter = 0;
	double th = log(40);

	for (i=0; i<(int)f0_data.size(); i++)
	{
		if (f0_data[i] >= th)
		{
			sum += f0_data[i];
			counter++;
		}
	}

	return sum/counter;
}

// ============================================================================
// Discrete Legendre Coefficient
// Note: L0 is simply syllable-level pitch mean!
// ============================================================================
void Legendre_poly_1(vector<double>& pch, vector<double>& L1)
{
	int i;
	int N = pch.size()-1;
	double normalizer = sqrt(12.0*N/(N+2));
	//cout<<"L1 normalizer = "<<normalizer<<endl;
	for (i=0; i<=N; i++)
		L1.push_back(normalizer * ((double)i/N-0.5));
}

void Legendre_poly_2(vector<double>& pch, vector<double>& L2)
{
	int i;
	int N = pch.size()-1;
	double normalizer = sqrt(180.0*pow(N,3.0)/((N-1)*(N+2)*(N+3)));
	//cout<<"L2 normalizer = "<<normalizer<<endl;
	for (i=0; i<=N; i++)
		L2.push_back( normalizer * ( pow((double)i/N, 2.0) - (double)i/N + (double)(N-1)/6/N ));
}

void Legendre_poly_3(vector<double>& pch, vector<double>& L3)
{
	int i;
	int N = pch.size()-1;
	double normalizer = sqrt(2800.0*pow(N,5.0)/((N-1)*(N-2)*(N+2)*(N+3)*(N+4)));
	//cout<<"L3 normalizer = "<<normalizer<<endl;
	for (i=0; i<=N; i++)
		L3.push_back( normalizer * ( pow((double)i/N, 3.0) - pow((double)i/N, 2.0)*3/2 
					+ (double)i*(6*N*N-3*N+2)/10/pow(N,3.0) - (double)(N-1)*(N-2)/20/N/N ));
}

void Legendre_coeff(vector<double>& pch, double* a)
{
	int i;
	int N = pch.size();
	vector<double> phi_1, phi_2, phi_3; // Legendre polynomial
	a[0] = a[1] = a[2] = 0; // Legendre coefficient
cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<endl;
	Legendre_poly_1(pch, phi_1);
	Legendre_poly_2(pch, phi_2);
	Legendre_poly_3(pch, phi_3);

	for (i=0; i<N; i++)
	{
		a[0] += phi_1[i]*pch[i];
		a[1] += phi_2[i]*pch[i];
		a[2] += phi_3[i]*pch[i];
cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<endl;
	}
	a[0] = a[0]/N;
	a[1] = a[1]/N;
	a[2] = a[2]/N;
cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<endl;
cout<<"finish legendre"<<endl;
	phi_1.clear();
	phi_2.clear();
	phi_3.clear();
}
