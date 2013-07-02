#ifndef CONTOURINTERPOLATER_H
#define CONTOURINTERPOLATER_H

using namespace std;

class ContourInterpolater
{
	public:
		virtual ~ContourInterpolater(){}
		virtual void Interpolate(vector<double>&) = 0;
};

#endif
