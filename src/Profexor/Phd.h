#ifndef PHD_H
#define PHD_H

#include "global.h"
#include "util.h"
#include "thread_util.h"
#include "Utterance.h"
#include "FeatureExtractor.h"
#include "PitchFeXor.h"
#include "SRIPitchFeXor.h"
#include "DurationFeXor.h"
#include "EnergyFeXor.h"
#include "PauseFeXor.h"
#include "FeatureSelector.h"
#include "SelectorKimchy.h"
#include "UtteranceBuilder.h"
#include "Config.h"

using namespace std;

/**
 * P-H-D for PtHreaD (*grin*)
*/
class Phd : public ThreadRunner
{
	public:
		Phd();
		virtual ~Phd();

		void Init(Config* c);
		void AssignJob(UtteranceInfo*, int);
		void* Run();
		void Report(ofstream&);
		void WriteDefinition(ofstream&);

	protected:
		void SelectFeatureSet(string featureSet, int numOfSegment);

	private:
		UtteranceBuilder* uBuilder;
		FeatureSelector* selector;
		vector<FeatureExtractor*> feaExtractors;
		UtteranceInfo* uttInfoPtr;
		int numUtterances;
		vector< vector< vector<double> > > features;
		bool extractDelta;
};

#endif
