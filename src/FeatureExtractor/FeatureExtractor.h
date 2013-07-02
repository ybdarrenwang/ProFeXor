#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "global.h"
#include "Utterance.h"
#include "mymath.h"
#include "util.h"

class FeatureExtractor
{
	public:
		virtual ~FeatureExtractor(){}
		virtual void Extract(Utterance*, vector< vector<double> >&, bool) = 0;
		virtual void PrintDefinition(vector<string>&) = 0;
		virtual void PrintDeltaDefinition(vector<string>&) = 0;

	protected:
		int numberOfFeatures;
};

#endif
