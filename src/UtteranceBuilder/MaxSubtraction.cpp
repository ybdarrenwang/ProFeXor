#include "MaxSubtraction.h"
#include "mymath.h"

using namespace std;

void MaxSubtraction::Normalize(vector<double>& points, UtteranceInfo* info)
{
	cout<<"Performing max subtraction"<<endl;

	int uttBeginTime = info->GetTimeLabel(0, 3);
	int uttEndTime = info->GetTimeLabel(info->GetNumUnit()-1, 0);
	int uttLength = uttEndTime-uttBeginTime;
	if (uttBeginTime < 0)
	{
		cerr<<"Error: begin of sentence has negative time label"<<endl;
		exit(1);
	}
	if (uttEndTime > points.size())
	{
		cerr<<"Error: end of sentence has exceeded pitch contour length"<<endl;
		exit(1);
	}

	double max = getMax(&points[uttBeginTime], uttLength);

	for (int p=0; p<uttBeginTime; ++p)
		points[p] = 0;
	for (int p=uttBeginTime; p<uttEndTime; ++p)
		points[p] -= max;
	for (int p=uttEndTime; p<points.size(); ++p)
		points[p] = 0;
}
