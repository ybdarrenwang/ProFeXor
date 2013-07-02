#include "Profexor.h"

using namespace std;

/**
* ProFeXor construction based on Config:\n
* 1. Select FeatureExtractor\n
* 2. Read .rec file into memory
*/
ProFeXor::ProFeXor(Config* c)
{
	cout<<endl<<"====== Constructing ProFeXor ======"<<endl;

	selector = 0;
	uBuilder = new UtteranceBuilder(c);
	extractDelta = c->GetExtractDelta();
	SelectFeatureSet(c->GetFeatureSet(), c->GetNumOfSegment());
	LoadREC(c->GetRECFile());
	fea = c->GetOutputFeatureFile();
	def = c->GetOutputDefinitionFile();
}

ProFeXor::~ProFeXor()
{
	if (selector != 0)
		delete selector;
	else
	{
		for (vector<FeatureExtractor*>::iterator x=feaExtractors.begin(); x!=feaExtractors.end(); x++)
			delete (*x);
	}
	delete uBuilder;
}

/**
 *1. For each Utterance, create empty vectors for containing features\n
 *2. For each FeatureExtractor, fill the corresponding features into the container
 */
void ProFeXor::ExtractFeature()
{
	ofstream ofs;
	if(!openFile(ofs, fea))
	{
		cerr<<"Error: cannot open "<<fea<<endl;
		exit(1);
	}
	cout<<endl<<"====== Writing "<<fea<<" ======"<<endl;

	vector<double> emptyFeatureContainer;
	Utterance* tmpUtterance;
	for (int i=0; i<utterancesInfo.size(); i++)
	{
		cout<<endl<<"===== Extracting features of the "<<i+1<<"-th utterance ====="<<endl;

		uBuilder->NewUtterance();
		uBuilder->BuildEnergyContour(&utterancesInfo[i]);
		uBuilder->BuildPitchContour(&utterancesInfo[i]);
		uBuilder->BuildAcousticUnit(&utterancesInfo[i]);
		tmpUtterance = uBuilder->GetResult();
		
		// create temp feature container for this utterance
		for (int syl=0; syl<tmpUtterance->GetNumberOfSyllables(); syl++)
			features.push_back(emptyFeatureContainer);

		// extract features
		for (int j=0; j<feaExtractors.size(); j++)
			feaExtractors[j]->Extract(tmpUtterance, features, extractDelta);

		WriteFeature(ofs, utterancesInfo[i].GetWaveFile(), utterancesInfo[i].GetF0File());
		features.clear();
	}
	ofs.close();	
}

void ProFeXor::WriteFeature(ofstream& ofs, string waveFile, string f0File)
{
	ofs<<"\""<<waveFile<<"\""<<endl;
	ofs<<"\""<<f0File<<"\""<<endl;
	for (int i=0; i<features.size(); i++)
	{
		if (selector != 0)
			selector->SelectFeature(features[i]);

		for (int j=0; j<features[i].size(); j++)
			ofs<<features[i][j]<<" ";
		ofs<<endl;
	}
	ofs<<"."<<endl;
}

void ProFeXor::WriteDefinition()
{
	ofstream ofs;
	if(!openFile(ofs, def))
	{
		cout<<"No feature definition file output"<<endl;
	}
	else
	{
		cout<<endl<<"====== Writing "<<def<<" ======"<<endl;
		vector<string> tmpDefinition;
		for (int i=0; i<feaExtractors.size(); i++)
		{
			feaExtractors[i]->PrintDefinition(tmpDefinition);
			if (extractDelta)
				feaExtractors[i]->PrintDeltaDefinition(tmpDefinition);
		}

		if (selector!=0)
			selector->SelectDefinition(tmpDefinition);

		for (int i=0; i<tmpDefinition.size(); i++)
			ofs<<tmpDefinition[i]<<endl;
	}
	ofs.close();
}

void ProFeXor::SelectFeatureSet(string featureSet, int numOfSegment)
{
	if( featureSet.find("F",0) != string::npos )
	{
		cout<<"Feature set by Che-Kuang Lin for disfluency detection is selected"<<endl;
		cout<<"note: other selection of features are overridden"<<endl;
		selector = new SelectorKimchy();
		selector->GetExtractors(feaExtractors);
		extractDelta = true;
	}
	else
	{
		if( featureSet.find("P",0) != string::npos )
		{
			cout<<"Pitch feature set selected"<<endl;
			FeatureExtractor* feaEx = new PitchFeXor(numOfSegment);
			feaExtractors.push_back(feaEx);
		}
		if( featureSet.find("S",0) != string::npos )
		{
			cout<<"SRI-stylized pitch feature set selected"<<endl;
			FeatureExtractor* feaEx = new SRIPitchFeXor(numOfSegment);
			feaExtractors.push_back(feaEx);
		}

		if (featureSet.find("D",0) != string::npos )
		{
			cout<<"Duration feature set selected"<<endl;
			FeatureExtractor* feaEx = new DurationFeXor();
			feaExtractors.push_back(feaEx);
		}

		if (featureSet.find("E",0) != string::npos )
		{
			cout<<"Energy feature set selected"<<endl;
			FeatureExtractor* feaEx = new EnergyFeXor(numOfSegment);
			feaExtractors.push_back(feaEx);
		}

		if (featureSet.find("X",0) != string::npos )
		{
			cout<<"Pause feature set selected"<<endl;
			FeatureExtractor* feaEx = new PauseFeXor();
			feaExtractors.push_back(feaEx);
		}
	}
}

void ProFeXor::LoadREC(string recFile)
{
	cout<<"Loading "<<recFile<<endl;

	ifstream ifs_recFile;
	if (!openFile(ifs_recFile, recFile))
	{
		cerr<<"Error(ProFeXor::LoadREC): cannot open "<<recFile<<endl;
		exit(1);
	}

	string line("");
	UtteranceInfo tmpUttInfo;
	while(!getLine(ifs_recFile, line))
	{
		if( line.find('"',0) != string::npos ) ///< file name found
		{
			removeChar(line, '"');
			if ( line.find(".wav") != string::npos) ///< wave file
				tmpUttInfo.SetWaveFile(line);
			else if ( line.find(".f0") != string::npos) ///< F0 file
				tmpUttInfo.SetF0File(line);
			else
				cerr<<"Warning(ProFeXor::LoadREC): un-recognizable file type "<<line<<endl;
		}
		else if (line == ".") ///< end of utterance
		{
			utterancesInfo.push_back(tmpUttInfo);
			tmpUttInfo.Clear();
		}
		else ///< utterance time label
		{
			vector<string> tokens = split(line);
			vector<int> tmpUnit;
			for (int i=0; i<(int)tokens.size(); i++)
			{
				if (tokens[i] == "-1")
					tmpUnit.push_back(-1);
				else
					tmpUnit.push_back((int)(atof(tokens[i].c_str())/100000.0));
			}
			tmpUttInfo.AddUnit(tmpUnit);
		}
	}
}
