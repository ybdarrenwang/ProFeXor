#ifndef MAXSUBTRACTION_H
#define MAXSUBTRACTION_H

#include "Normalizer.h"

using namespace std;

class MaxSubtraction : public Normalizer
{
	public:
		void Normalize(vector<double>&, UtteranceInfo*);
};

#endif
