#include "FeatureSelector.h"

void FeatureSelector::SelectFeature(vector<double>& features)
{
	vector<double> tmpSelectedFeatures;
	for (int i=0; i<myFeatureSet.size(); i++)
		tmpSelectedFeatures.push_back(features[myFeatureSet[i]-1]);
	features.clear();
	features = tmpSelectedFeatures;
}

void FeatureSelector::SelectDefinition(vector<string>& definitions)
{
	vector<string> tmpSelectedDefinitions;
	for (int i=0; i<myFeatureSet.size(); i++)
		tmpSelectedDefinitions.push_back(definitions[myFeatureSet[i]-1]);
	definitions.clear();
	definitions = tmpSelectedDefinitions;
}
