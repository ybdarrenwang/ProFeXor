#include "Phd.h"

using namespace std;

Phd::Phd()
{
	DUMP("\n< New Phd...... >");
	uttInfoPtr = 0;
	uBuilder = 0;
	selector = 0;
}

Phd::~Phd()
{
	if (selector != 0)
		delete selector;
	for (vector<FeatureExtractor*>::iterator x=feaExtractors.begin(); x!=feaExtractors.end(); x++)
		delete (*x);
	if (uBuilder != 0)
		delete uBuilder;
}

void Phd::Init(Config* c)
{
	uBuilder = new UtteranceBuilder(c);
	extractDelta = c->GetExtractDelta();
	SelectFeatureSet(c->GetFeatureSet(), c->GetNumOfSegment());
}


void Phd::SelectFeatureSet(string featureSet, int numOfSegment)
{
	DUMP("\n< Select feature sets >");

	if( featureSet.find("F",0) != string::npos )
	{
		DUMP("- Feature set by Che-Kuang Lin for disfluency detection is selected");
		DUMP("  (note: other selection of features are overridden)");
		selector = new SelectorKimchy();
		selector->InstallExtractors(feaExtractors);
		extractDelta = true;
	}
	else
	{
		if( featureSet.find("P",0) != string::npos )
		{
			DUMP("- Pitch feature set selected");
			FeatureExtractor* feaEx = new PitchFeXor(numOfSegment);
			feaExtractors.push_back(feaEx);
		}
		if( featureSet.find("S",0) != string::npos )
		{
			DUMP("- SRI-stylized pitch feature set selected");
			FeatureExtractor* feaEx = new SRIPitchFeXor(numOfSegment);
			feaExtractors.push_back(feaEx);
		}

		if (featureSet.find("D",0) != string::npos )
		{
			DUMP("- Duration feature set selected");
			FeatureExtractor* feaEx = new DurationFeXor();
			feaExtractors.push_back(feaEx);
		}

		if (featureSet.find("E",0) != string::npos )
		{
			DUMP("- Energy feature set selected");
			FeatureExtractor* feaEx = new EnergyFeXor(numOfSegment);
			feaExtractors.push_back(feaEx);
		}

		if (featureSet.find("X",0) != string::npos )
		{
			DUMP("- Pause feature set selected");
			FeatureExtractor* feaEx = new PauseFeXor();
			feaExtractors.push_back(feaEx);
		}
	}
}

void Phd::AssignJob(UtteranceInfo* u, int numUtt)
{
	uttInfoPtr = u;
	numUtterances = numUtt;
}

/**
 *1. For each Utterance, create empty vectors for containing features\n
 *2. For each FeatureExtractor, fill the corresponding features into the container
 */
void* Phd::Run()
{
	for (int i=0; i<numUtterances; ++i)
	{
		UtteranceInfo* uttInfo = &(uttInfoPtr[i]);

		// build utterance based on the information
		cout<<"< Load utterance >"<<endl;
		uBuilder->NewUtterance();
		uBuilder->BuildEnergyContour(uttInfo);
		uBuilder->BuildPitchContour(uttInfo);
		uBuilder->BuildAcousticUnit(uttInfo);
		Utterance* tmpUtterance = uBuilder->GetResult();

		// create temp feature container for this utterance
		features.push_back(vector< vector<double> >());
		for (int syl=0; syl<tmpUtterance->GetNumberOfSyllables(); syl++)
			features[i].push_back(vector<double>());

		// extract features
		cout<<"< Extract features >"<<endl;
		for (int j=0; j<feaExtractors.size(); ++j)
			feaExtractors[j]->Extract(tmpUtterance, features[i], extractDelta);

		if (selector != 0)
		{
			for (int j=0; j<features[i].size(); ++j)
				selector->SelectFeature(features[i][j]);
		}
	}
}

void Phd::Report(ofstream& ofs)
{
	for (int i=0; i<numUtterances; ++i)
	{
		UtteranceInfo* uttInfo = &(uttInfoPtr[i]);

		ofs<<"\""<<uttInfo->GetWaveFile()<<"\""<<endl;
		ofs<<"\""<<uttInfo->GetF0File()<<"\""<<endl;
		for (int j=0; j<features[i].size(); ++j)
		{
			for (int k=0; k<features[i][j].size(); k++)
				ofs<<features[i][j][k]<<" ";
			ofs<<endl;
		}
		ofs<<"."<<endl;
	}
}

void Phd::WriteDefinition(ofstream& ofs)
{
	vector<string> tmpDefinition;
	for (int i=0; i<feaExtractors.size(); ++i)
	{
		feaExtractors[i]->PrintDefinition(tmpDefinition);
		if (extractDelta)
			feaExtractors[i]->PrintDeltaDefinition(tmpDefinition);
	}

	if (selector!=0)
		selector->SelectDefinition(tmpDefinition);

	for (int i=0; i<tmpDefinition.size(); ++i)
		ofs<<tmpDefinition[i]<<endl;
}
