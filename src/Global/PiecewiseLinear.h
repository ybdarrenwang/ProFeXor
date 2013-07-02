#ifndef PIECEWISELINEARFUNC_H
#define PIECEWISELINEARFUNC_H

#include "mymath.h"

using namespace std;

class PiecewiseLinear
{
	public:
		PiecewiseLinear(int);
		virtual ~PiecewiseLinear();

		void DivideSegment(double*, int, double);
		void OptApprox(vector<double>&);
		void GetValue(vector<double>&);

	private:
		int num_seg; ///< number of segments
		int* knots;
		double** coeff;

		double sse; ///< sum of square error
		int* tmpSegLen;
		double** tmpCoeff;
		int remainingLen;

		// [n][0]: the sse of linear regression of the segment with length n to end of input\n
		// [n][1]: the optimal slope of linear regression of the segment with length n to end of input\n
		// [n][2]: the optimal bias of linear regression of the segment with length n to end of input
		double** tmpLastSegCoeff;
};

#endif
