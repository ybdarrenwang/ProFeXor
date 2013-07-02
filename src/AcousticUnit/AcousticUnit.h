#ifndef ACOUSTICUNIT_H
#define ACOUSTICUNIT_H

#include "global.h"

using namespace std;

class AcousticUnit
{
	public:
		virtual ~AcousticUnit(){}

		int GetBeginTime() const{return beginTime;}
		int GetEndTime() const{return endTime;}
		int GetDuration() const{return endTime-beginTime;}
		vector<double> GetPitchContour() const{return pitchContour;}
		vector<double> GetEnergyContour() const{return energyContour;}
		virtual void SetPitchContour(vector<double> c){pitchContour = c;}
		virtual void SetEnergyContour(vector<double> c){energyContour = c;}

	protected:
		int beginTime;
		int endTime;
		vector<double> pitchContour;
		vector<double> energyContour;
};

#endif
