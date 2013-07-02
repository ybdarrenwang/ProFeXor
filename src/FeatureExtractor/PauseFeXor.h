#ifndef PAUSEFEXOR_H
#define PAUSEFEXOR_H

#include "global.h"
#include "FeatureExtractor.h"

using namespace std;

class PauseFeXor : public FeatureExtractor
{
	public:
		void Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta);
		void PrintDefinition(vector<string>&);
		void PrintDeltaDefinition(vector<string>&);

	protected:
		void InitializeFeature(int numSyllable);
		void GetFeature(Utterance* u);
		void SaveFeature(vector< vector<double> >& features, int numSyllable);
		void ResetFeature();

		void InitializeDeltaFeature(int numSyllable);
		void GetDeltaFeature(int numSyllable);
		void SaveDeltaFeature(vector< vector<double> >& features, int numSyllable);
		void ResetDeltaFeature();

	private:
		// 12 features
		vector<double> ratioAvgSylDurationFromPrevUntilNextPause; ///< the ratio of average duration of the syllables, before and after a break, calculated from the nearest short pause
		vector<double> ratio_prevSyl_avgSylDurationFromPrevPause; ///<Pre-boundary lengthening feature: The syllable duration before this boundary, divide by the average syllable duration calculted from previous pause to this boundary.
		vector<double> mulPauseSylDuration; ///< the product between pause duration and the duration of the following syllable
		vector<double> mulSylPauseDuration; ///< the product between pause duration and the duration of the preceding syllable
		vector<double> ratioPauseSylDuration; ///< the following syllable duration divided by pause duration
		vector<double> ratioSylPauseDuration; ///< the preceding syllable duration divided by pause duration
		vector<double> stdDev_mulPauseSylDuration;
		vector<double> stdDev_mulSylPauseDuration;
		vector<double> stdDev_ratioPauseSylDuration;
		vector<double> stdDev_ratioSylPauseDuration;
		vector<double> sumSylDurationFromPrevPause; ///< distance from the last pause (in frame)
		vector<double> numSylFromPrevPause; ///< distance from the last pause (in syllable)

		// 10 delta-features
		vector<double> d_ratioAvgSylDurationFromPrevUntilNextPause;
		vector<double> d_ratio_prevSyl_avgSylDurationFromPrevPause;
		vector<double> d_mulPauseSylDuration;
		vector<double> d_mulSylPauseDuration;
		vector<double> d_ratioPauseSylDuration;
		vector<double> d_ratioSylPauseDuration;
		vector<double> d_stdDev_mulPauseSylDuration;
		vector<double> d_stdDev_mulSylPauseDuration;
		vector<double> d_stdDev_ratioPauseSylDuration;
		vector<double> d_stdDev_ratioSylPauseDuration;
};

#endif
