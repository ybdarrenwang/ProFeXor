#ifndef PROFEXOR_H
#define PROFEXOR_H

#include "global.h"
#include "Config.h"
#include "util.h"
#include "Utterance.h"
#include "UtteranceInfo.h"
#include "FeatureExtractor.h"
#include "PitchFeXor.h"
#include "SRIPitchFeXor.h"
#include "DurationFeXor.h"
#include "EnergyFeXor.h"
#include "PauseFeXor.h"
#include "FeatureSelector.h"
#include "SelectorKimchy.h"
#include "UtteranceBuilder.h"

using namespace std;

class ProFeXor
{
	public:
		ProFeXor(Config*);
		~ProFeXor();
		void ExtractFeature();
		void WriteFeature(ofstream&, string, string);
		void WriteDefinition();

	protected:
		void SelectFeatureSet(string featureSet, int numOfSegment);
		void LoadREC(string recFile);

	private:
		UtteranceBuilder* uBuilder;
		vector<UtteranceInfo> utterancesInfo;
		vector<FeatureExtractor*> feaExtractors;
		vector< vector<double> > features;
		FeatureSelector* selector;
		bool extractDelta;
		string fea; ///< output feature file
		string def; ///< output definition file
};

#endif
