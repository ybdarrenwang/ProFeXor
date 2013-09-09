#include "SRIStylizer.h"
#include "mymath.h"

vector<double> PiecewiseLinearApprox(vector<double> rawpitch)
{
	vector<double> axe;
	Vector_Axe(rawpitch.size(), axe);

	int num_seg;
	if (rawpitch.size() >= 12)
		num_seg = 3;	
	else if (rawpitch.size() >= 7)
		num_seg = 2;
	else
		num_seg = 1;

	PiecewiseLinear pwl(num_seg);
	pwl.OptApprox(rawpitch);

	vector<double> styline;
	pwl.GetValue(styline);

	return styline;
}

void SRIStylizer::Stylize(Utterance* u)
{
	DUMP(__PRETTY_FUNCTION__);

	vector<double> tmpPitchContour = u->GetPitchContour();
	int pitchContourLength = tmpPitchContour.size();
	vector<double> stylizedPitchContour = Vector_Zeros(pitchContourLength);
	vector<double> tmp_axe;
	for(int s=0; s<u->GetNumberOfSyllables(); s++)
	{
		if(u->GetSyllable(s)->GetPrevPause() != 0 || s==0)
		{
			// the region for stylization includes all the frames between two pauses
			int stylizeRegionBeginTime = u->GetSyllable(s)->GetNuclei()->GetBeginTime();
			s++;
			while(s!=u->GetNumberOfSyllables() && u->GetSyllable(s)->GetPrevPause()==0) s++;
			s--;
			int stylizeRegionEndTime = u->GetSyllable(s)->GetNuclei()->GetEndTime();

			vector<double> tmpStylizedPitchContour = PiecewiseLinearApprox(subVec(tmpPitchContour,stylizeRegionBeginTime,stylizeRegionEndTime-1));

			if(!tmpStylizedPitchContour.empty())
			{
				for(int i=0;i<stylizeRegionEndTime-stylizeRegionBeginTime;i++)
					stylizedPitchContour[stylizeRegionBeginTime+i]=tmpStylizedPitchContour[i];
			}
		}
	}

	u->SetPitchContour(stylizedPitchContour);
}
