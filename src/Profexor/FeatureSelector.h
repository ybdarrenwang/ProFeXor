#ifndef FEATURESELECTOR_H
#define FEATURESELECTOR_H

#include "FeatureExtractor.h"

class FeatureSelector
{
	public:
		virtual ~FeatureSelector(){}
		virtual void SelectFeature(vector<double>&);
		virtual void SelectDefinition(vector<string>&);
		virtual void InstallExtractors(vector<FeatureExtractor*>&) = 0;

	protected:
		vector<int> myFeatureSet;
};

#endif
