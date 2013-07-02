#ifndef MOVINGWINDOWSMOOTHER
#define MOVINGWINDOWSMOOTHER

#include "mymath.h"
#include "ContourSmoother.h"

using namespace std;

class MovingWindowSmoother : public ContourSmoother
{
	public:
		MovingWindowSmoother(int w){windowSize = w;}
		void Smooth(vector<double>& p)
		{
			cout<<"Performing moving window smoothing with window size="<<windowSize<<endl;
			vector<double> average;
			Vector_WindowAvg(p, windowSize, average);
			p = average;
		}

	private:
		int windowSize;
};

#endif
