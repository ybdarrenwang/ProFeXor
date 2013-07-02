#ifndef CONTOURSMOOTHER_H
#define CONTOURSMOOTHER_H

using namespace std;

class ContourSmoother
{
	public:
		virtual ~ContourSmoother(){}
		virtual void Smooth(vector<double>&) = 0;
};

#endif
