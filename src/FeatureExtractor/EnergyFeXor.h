#ifndef ENERGYFEXOR_H
#define ENERGYFEXOR_H

#include "global.h"
#include "FeatureExtractor.h"
#include "util.h"

class EnergyFeXor : public FeatureExtractor
{
	public:
		EnergyFeXor(int i) {numOfSegment = i;}
		void Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta);
		void PrintDefinition(vector<string>&);
		void PrintDeltaDefinition(vector<string>&);

	protected:
		void InitializeFeature(int numSyllable);
		void GetFeature(Utterance* u);
		void SaveFeature(vector< vector<double> >& features, int numSyllable);
		void ResetFeature(int);

		void InitializeDeltaFeature(int numSyllable);
		void GetDeltaFeature(int numSyllable);
		void SaveDeltaFeature(vector< vector<double> >& features, int numSyllable);
		void ResetDeltaFeature(int);

	private:
		int numOfSegment;

		// 10+numOfSegment features
		vector<double> begin; ///< the log-energy of the first frame of this character
		vector<double> end;
		vector<double> slopeBegin;
		vector<double> slopeEnd;
		vector<double> min;
		vector<double> max;
		vector<double> avg; ///< vowel average frame energy
		vector<double> avgSlope;
		vector<double> avgNuclei;
		vector<double> totalNuclei;
		vector< vector<double> > segment; ///< segment average frame energy

		// 15+(2*numOfSegment-1) delta-features
		vector<double> dBegin;
		vector<double> dEnd;
		vector<double> dbe; ///< begin - end
		vector<double> ddbe;
		vector<double> dSlopeBegin;
		vector<double> dSlopeEnd;
		vector<double> dbeSlope;
		vector<double> ddbeSlope;
		vector<double> dMin;
		vector<double> dMax;
		vector<double> range;
		vector<double> dRange;
		vector<double> dAvg;
		vector<double> dAvgSlope;
		vector<double> dAvgNuclei;
		vector< vector<double> > dSyl_Segment;
		vector< vector<double> > dSeg_Segment;
};

#endif
