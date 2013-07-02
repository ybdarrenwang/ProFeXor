#ifndef UTTERANCEINFO_H
#define UTTERANCEINFO_H

#include "global.h"

using namespace std;

class UtteranceInfo
{
	public:
		void SetWaveFile(string s){wavFile = s;}
		void SetF0File(string s){f0File = s;}
		string GetWaveFile(){return wavFile;}
		string GetF0File(){return f0File;}
		vector< vector<int> > GetTimeLabel(){return timeLabel;}
		int GetTimeLabel(int i, int j){return timeLabel[i][j];}
		int GetNumUnit(){return timeLabel.size();}
		void AddUnit(vector<int>& u){timeLabel.push_back(u);}
		void Clear(){
			wavFile = "";
			f0File = "";
			for (int i=0; i<timeLabel.size(); i++)
				timeLabel[i].clear();
			timeLabel.clear();
		}

	protected:
		string wavFile;
		string f0File;
		vector< vector<int> > timeLabel;
};

#endif
