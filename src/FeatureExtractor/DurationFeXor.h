#ifndef DURATIONFEXOR_H
#define DURATIONFEXOR_H

#include "FeatureExtractor.h"

class DurationFeXor : public FeatureExtractor
{
	public:
		void Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta);
		void PrintDefinition(vector<string>&);
		void PrintDeltaDefinition(vector<string>&);

	protected:
		void InitializeFeature(int numSyllable);
		void InitializeDeltaFeature(int numSyllable);
		void GetIntraSyllableFeature(Utterance* u);
		void GetCrossSyllableFeature(int numSyllable);
		void GetDeltaFeature(int numSyllable);
		void SaveFeature(vector< vector<double> >& features, int numSyllable);
		void SaveDeltaFeature(vector< vector<double> >& features, int numSyllable);
		void ResetFeature();
		void ResetDeltaFeature();

	private:
		double avgSylDuration;

		// 15 features
		vector<double> sylDur; ///< syllable duration
		vector<double> n_sylDur; ///< syllable duration normalized by utterance mean syllable duration
		vector<double> avgWithPrevSylDur; ///< average of 2 adjacent syllable duration
		vector<double> n_avgWithPrevSylDur;
		vector<double> ratioSylDur_SylBesideBoundary; ///< the duration ratio of 2 adjacent syllables
		vector<double> ratioSylDur_OneSylApartFromBoundary; ///< the duration ratio of two syllables, 1 syllable apart from a boundary
		vector<double> ratioSylDur_TwoSylApartFromBoundary;
		vector<double> ratioAvgSylDurAcrossBoundary; ///< the ratio of average duration of the syllables, before and after a break, calculated from sentence boundary
		vector<double> stdDev_sylDur; ///< the standard deviation of sylDur
		vector<double> onsetDur;
		vector<double> n_onsetDur;
		vector<double> nucleiDur;
		vector<double> n_nucleiDur;
		vector<double> codaDur;
		vector<double> n_codaDur;

		// 9 delta-features
		vector<double> d_sylDur;
		vector<double> d_n_sylDur;
		vector<double> d_avgWithPrevSylDur;
		vector<double> d_n_avgWithPrevSylDur;
		vector<double> d_ratioSylDur_SylBesideBoundary;
		vector<double> d_ratioSylDur_OneSylApartFromBoundary;
		vector<double> d_ratioSylDur_TwoSylApartFromBoundary;
		vector<double> d_ratioAvgSylDurAcrossBoundary;
		vector<double> d_stdDev_sylDur;
};

#endif
