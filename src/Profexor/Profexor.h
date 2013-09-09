#ifndef PROFEXOR_H
#define PROFEXOR_H

#include "Phd.h"
#include "UtteranceInfo.h"

using namespace std;

class ProFeXor
{
	public:
		ProFeXor(Config*);
		void ExtractFeature();
		void WriteDefinition();

	protected:
		void LoadREC(string recFile);

	private:
		vector<Phd> students;
		vector<UtteranceInfo> utteranceInfo;
		string feaFile; ///< output feature file
		string defFile; ///< output definition file
};

#endif
