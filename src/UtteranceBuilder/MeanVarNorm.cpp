#include "MeanVarNorm.h"
#include "mymath.h"

void MeanVarNorm::Normalize(vector<double>& points, UtteranceInfo* info)
{
	cout<<"Performing mean subtraction"<<endl;

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

	double mean = getMean(&points[uttBeginTime], uttLength);
	double stdDev = getSD(&points[uttBeginTime], uttLength);

	for (int p=0; p<uttBeginTime; ++p)
		points[p] = 0;
	for (int p=uttBeginTime; p<uttEndTime; ++p)
		points[p]=(points[p]-mean)/stdDev;
	for (int p=uttEndTime; p<points.size(); ++p)
		points[p] = 0;
}
