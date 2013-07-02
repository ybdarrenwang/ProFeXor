#include "MovingWindowMeanSub.h"
#include "mymath.h"

void MovingWindowMeanSub::Normalize(vector<double>& points, UtteranceInfo* info)
{
	cout<<"Performing moving window mean subtraction with window size="<<windowSize<<endl;

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

	vector<double> average;
	Vector_WindowAvg(points, windowSize, average);
	for (int p=0; p<points.size(); p++)
		points[p]-=average[p];
}
