#include "CubicSplineInterpolater.h"

using namespace std;

void CubicSplineInterpolater::Interpolate(vector<double>& p)
{
	cout<<"Performing Cubic Spline interpolation"<<endl;
	vector<double> availablePoints, availableData;
	int originalLength = p.size();
	for (int i=0; i<originalLength; i++)
	{
		if (p[i] != 0)
		{
			availableData.push_back(p[i]);
			availablePoints.push_back((double)i);
		}
	}

	spline.CubicSpline(availablePoints, availableData);
	spline.GetPPValue(availablePoints.front(), availablePoints.back(), p);

	// recover the beginning and end silence cut by GetPPValue
	// the beginning of contour: copy the first available value backward
	p.insert(p.begin(), (int)availablePoints.front(), availableData.front());
	// the end of contour: copy the last available value forward
	p.insert(p.end(), originalLength-(int)availablePoints.back()-1, availableData.back());

	// clear memory
	spline.Reset();
}
