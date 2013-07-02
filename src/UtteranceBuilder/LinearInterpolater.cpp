#include "LinearInterpolater.h"

void LinearInterpolater::Interpolate(vector<double>& p)
{
	cout<<"Performing linear interpolation"<<endl;

	double beginValue = 0, endValue = 0;
	int begin = 0, end = 0;
	bool interpolating = false;
	for (int i=0; i<p.size(); i++)
	{
		if (p[i] == 0 && !interpolating) // beginning of undefined contour
		{
			if (i>0)
			{
				begin = i;
				beginValue = p[i-1];
			}
			interpolating = true;
		}
		else if (p[i] > 0 && interpolating) // the end of undefined contour
		{
			// the beginning of contour: copy the first available value backward
			if (beginValue == 0)
			{
				for (int j=0; j<i; ++j)
					p[j] = p[i];
			}
			else // not the beginning of contour
			{
				end = i-1;
				endValue = p[i];

				// start interpolating
				double step = (endValue-beginValue)/(end-begin+2);
				for (int j=begin; j<=end; ++j)
					p[j]=p[j-1]+step;
			}
			interpolating = false;
		}
	}

	if (begin!=0) // the end of contour: copy the last available value forward
	{
		for (int j=begin; j<p.size(); ++j)
			p[j] = p[begin-1];
	}
}
