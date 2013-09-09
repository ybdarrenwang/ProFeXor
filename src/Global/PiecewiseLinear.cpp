#include "PiecewiseLinear.h"

PiecewiseLinear::PiecewiseLinear(int tmp_num_seg)
{
	num_seg = tmp_num_seg;
	
	knots = new int[num_seg+1];
	coeff = new double*[num_seg];
	for (int i=0; i<num_seg; i++)
		coeff[i] = new double[2];
	sse = 1e38;
}

PiecewiseLinear::~PiecewiseLinear()
{
	delete [] knots;
	for (int i=0; i<num_seg; i++)
		delete [] coeff[i];
	delete [] coeff;
}

/**
 * Recursively divide the input function into piecewise linear function 
 * and calculate the sum of square error.\n
 * Note: We can accelerate the computation by memorizing the result of 
 * segments between some specific begin and end frames.\n
 * However, because we initiate the recursive call based on the end time of 
 * 1st segment, it is trivial that the 1st and 2nd segments won't run into 
 * repeated begin and end time. For our application of 3-segmented linear 
 * approximation, I only memorize the 3-rd segment in 
 * PiecewiseLinear::tmpLastSegCoeff.
*/
//void PiecewiseLinear::DivideSegment(vector<double>& x, int seg_index, double sse, int* tmpSegLen, double** tmpCoeff)
void PiecewiseLinear::DivideSegment(double* x, int seg_index, double tmp_sse)
{
	// The last segment:
	// Calculate the sum of square error, and update the
	// piecewise linear function if the error is minimal
	if (seg_index == num_seg-1)
	{
		if (tmpLastSegCoeff[remainingLen][0] == -1)
		{
			LinearRegression(x, remainingLen, tmpCoeff[seg_index]);
			tmpLastSegCoeff[remainingLen][0] = SquareError(x, remainingLen, tmpCoeff[seg_index]);
			tmpLastSegCoeff[remainingLen][1] = tmpCoeff[seg_index][0];
			tmpLastSegCoeff[remainingLen][2] = tmpCoeff[seg_index][1];
		}
		else
		{
			tmpCoeff[seg_index][0] = tmpLastSegCoeff[remainingLen][1];
			tmpCoeff[seg_index][1] = tmpLastSegCoeff[remainingLen][2];
		}
		tmp_sse += tmpLastSegCoeff[remainingLen][0];

		if (tmp_sse < sse)
		{
			sse = tmp_sse;
			for (int i=0; i<num_seg; i++)
			{
				coeff[i][0] = tmpCoeff[i][0];
				coeff[i][1] = tmpCoeff[i][1];
			}
			for (int i=1; i<num_seg; i++)
				knots[i] = knots[i-1] + tmpSegLen[i-1];
		}
	}
	else // cut down the seg_index-th segment, and pass the remaining contour to the next recursion
	{
		for (tmpSegLen[seg_index]=1; tmpSegLen[seg_index]<remainingLen-num_seg+seg_index+1; tmpSegLen[seg_index]++)
		{
			LinearRegression(x, tmpSegLen[seg_index], tmpCoeff[seg_index]);
			if (tmpCoeff[seg_index][0] != 1e38)
			{
				double next_sse = tmp_sse+SquareError(x, tmpSegLen[seg_index], tmpCoeff[seg_index]);
				if (next_sse < sse)
				{
					remainingLen -= tmpSegLen[seg_index];
					DivideSegment(&(x[tmpSegLen[seg_index]]), seg_index+1, next_sse);
					remainingLen += tmpSegLen[seg_index];
				}
			}
		}
	}
}

void PiecewiseLinear::OptApprox(vector<double>& x)
{
	// allocate temp memory
	tmpSegLen = new int[num_seg];
	tmpCoeff = new double*[num_seg];
	for (int i=0; i<num_seg; i++)
		tmpCoeff[i] = new double[2];

	tmpLastSegCoeff = new double*[x.size()+1];
	for (int i=0; i<=x.size(); i++)
	{
		tmpLastSegCoeff[i] = new double[3];
		tmpLastSegCoeff[i][0] = -1;
	}

	// run
	knots[0] = 0;
	knots[num_seg] = x.size()-1;
	remainingLen = x.size();
	DivideSegment(&(x[0]), 0, 0);

	// free temp memory
	delete [] tmpSegLen;
	for (int i=0; i<num_seg; i++)
		delete [] tmpCoeff[i];
	delete [] tmpCoeff;
	for (int i=0; i<=x.size(); i++)
		delete [] tmpLastSegCoeff[i];
	delete [] tmpLastSegCoeff;

	if (sse >= 1e38)
	{
		cerr<<"[Error] fail to find optimal piecewise linear function"<<endl;
		exit(1);
	}
}

void PiecewiseLinear::GetValue(vector<double>& y)
{
	y.clear();
	vector<double> axe;
	for (int s=0; s<num_seg; s++)
	{
		Vector_Axe(knots[s+1]-knots[s], axe);
		for (int i=0; i<axe.size(); i++)
			y.push_back(coeff[s][0]*axe[i] + coeff[s][1]);
	}
	y.push_back(coeff[num_seg-1][0]*axe.size() + coeff[num_seg-1][1]);
}
