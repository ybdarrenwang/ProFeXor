#ifndef SRIPITCHFEXOR_H
#define SRIPITCHFEXOR_H

#include "PitchFeXor.h"
#include "SRIStylizer.h"

class SRIPitchFeXor : public PitchFeXor
{
	public:
		SRIPitchFeXor(int i):PitchFeXor(i){stylizer = new SRIStylizer();}
		~SRIPitchFeXor(){delete stylizer;}
		void Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta);
		void PrintDefinition(vector<string>& ofs);
		void PrintDeltaDefinition(vector<string>& ofs);
};

#endif
