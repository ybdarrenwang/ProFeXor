#ifndef PITCHFEXOR_H
#define PITCHFEXOR_H

#include "FeatureExtractor.h"
#include "PitchContourStylizer.h"

/**
* Pitch Feature Extractor\n
* \n
* abbr:\n
*   dSyl: difference of feature across neighborint syllable\n
*   dSeg: differnece of feature across neighboring segment\n
*/
class PitchFeXor : public FeatureExtractor
{
	public:
		PitchFeXor(int i){stylizer = 0; numOfSegment = i;}
		virtual void Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta);
		virtual void PrintDefinition(vector<string>&);
		virtual void PrintDeltaDefinition(vector<string>&);

	protected:
		void InitializeFeature(int numSyllable);
		void GetFeature(Utterance* u);
		void SaveFeature(vector< vector<double> >& features, int numSyllable);
		void ResetFeature(int numOfSyllable);

		void InitializeDeltaFeature(int numSyllable);
		void GetDeltaFeature(int numSyllable);
		void SaveDeltaFeature(vector< vector<double> >& features, int numSyllable);
		void ResetDeltaFeature(int numOfSyllable);

		PitchContourStylizer* stylizer;
		int numOfSegment;

		// 14+numOfSegment*3 features
		vector<double> begin;
		vector<double> end;
		vector<double> slopeBegin;
		vector<double> slopeEnd;
		vector<double> min;
		vector<double> n_min;
		vector<double> max;
		vector<double> n_max;
		vector<double> avgPitch; ///< average vowel pitch
		vector<double> slopePitch;
		vector<double> avgSlope; ///< average curvature
		vector< vector<double> > Legendre;
		vector< vector<double> > avgSegmentPitch;
		vector< vector<double> > n_avgSegmentPitch;
		vector< vector<double> > slopeSegmentPitch;

		// delta-features
		vector<double> d_begin;
		vector<double> d_end;
		vector<double> dbe;
		vector<double> d_dbe;
		vector<double> d_slopeBegin;
		vector<double> d_slopeEnd;
		vector<double> dbeSlope;
		vector<double> d_dbeSlope;
		vector<double> d_min;
		vector<double> d_max;
		vector<double> range;
		vector<double> d_range;
		vector<double> d_avgPitch;
		vector<double> d_slopePitch;
		vector<double> d_avgSlope;
		vector< vector<double> > dSyl_avgSegmentPitch;
		vector< vector<double> > dSeg_avgSegmentPitch;
		vector< vector<double> > dSyl_slopeSegmentPitch;
		vector< vector<double> > dSeg_slopeSegmentPitch;
};

#endif
