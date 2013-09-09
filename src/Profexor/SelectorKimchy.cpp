#include "SelectorKimchy.h"

SelectorKimchy::SelectorKimchy()
{
	int tmpFeatureSet[] = {1,2,3,4,9,11,12,13,14,15,16,17,18,19,22,23,24,26,27,28,29,30,35,37,38,39,40,41,42,43,44,45,48,49,50,52,53,54,55,56,57,58,59,60,61,68,69,70,71,72,73,74,75,76,77,78,79,80,83,84,87,88,89,90,91,92,93,94,97,98,99,100,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123};
	myFeatureSet = vector<int>(tmpFeatureSet, tmpFeatureSet+sizeof(tmpFeatureSet)/sizeof(int));
}

void SelectorKimchy::InstallExtractors(vector<FeatureExtractor*>& myExtractors)
{
	myExtractors.push_back(new PitchFeXor(0)); // 18 features
	myExtractors.push_back(new SRIPitchFeXor(0)); // 18 features
	myExtractors.push_back(new DurationFeXor()); // 18 features
	myExtractors.push_back(new EnergyFeXor(0)); // 18 features
	myExtractors.push_back(new PauseFeXor()); // 22 features
}
