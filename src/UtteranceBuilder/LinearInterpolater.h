#ifndef	LINEARINTERPOLATER
#define	LINEARINTERPOLATER

#include "global.h"
#include "ContourInterpolater.h"

using namespace std;

class LinearInterpolater : public ContourInterpolater
{
	public:
		void Interpolate(vector<double>&);
};

#endif
