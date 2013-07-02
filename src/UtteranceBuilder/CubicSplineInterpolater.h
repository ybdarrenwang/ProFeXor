#ifndef	CUBICSPLINEINTERPOLATER
#define	CUBICSPLINEINTERPOLATER

#include "global.h"
#include "Spline.h"
#include "ContourInterpolater.h"

using namespace std;

class CubicSplineInterpolater : public ContourInterpolater
{
	public:
		void Interpolate(vector<double>&);

	private:
		Spline spline;
};

#endif
