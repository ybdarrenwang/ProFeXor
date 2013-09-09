#include "Profexor.h"

using namespace std;

ProFeXor::ProFeXor(Config* c)
{
	DUMP("\n< New ProFeXor...... >");

	LoadREC(c->GetRECFile());
	feaFile = c->GetOutputFeatureFile();
	defFile = c->GetOutputDefinitionFile();

	for (int i=0; i<c->GetNumOfThread(); ++i)
		students.push_back(Phd());

	for (int i=0; i<students.size(); ++i)
		students[i].Init(c);
}

/**
 * Split utterances into batches of same size, and assign them to students
*/
void ProFeXor::ExtractFeature()
{
	int batchSize;
	int beginUtt = 0;
	for (int i=0; i<students.size(); ++i)
	{
		batchSize = (int)floor((float)((i+1)*utteranceInfo.size())/students.size()) - beginUtt;
		students[i].AssignJob(&utteranceInfo[beginUtt], batchSize);
		beginUtt+=batchSize;
	}

	CastThreads(students);

	// report
	cout<<"< Write "<<feaFile<<" >"<<endl;

	ofstream ofs;
	if(!openFile(ofs, feaFile))
	{
		cerr<<"[Error] cannot open "<<feaFile<<endl;
		exit(1);
	}

	for (int i=0; i<students.size(); ++i)
		students[i].Report(ofs);

	ofs.close();
}

void ProFeXor::WriteDefinition()
{
	ofstream ofs;
	if(!openFile(ofs, defFile))
	{
		cout<<"< No feature definition file output >"<<endl;
		return;
	}
	
	cout<<"< Write "<<defFile<<" >"<<endl;
	students[0].WriteDefinition(ofs);
	
	ofs.close();
}

void ProFeXor::LoadREC(string recFile)
{
	cout<<"< Read "<<recFile<<" >"<<endl;

	ifstream ifs_recFile;
	if (!openFile(ifs_recFile, recFile))
	{
		cerr<<"[Error] cannot open "<<recFile<<endl;
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
				cerr<<"[Warning] un-recognizable file type "<<line<<endl;
		}
		else if (line == ".") ///< end of utterance
		{
			utteranceInfo.push_back(tmpUttInfo);
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
