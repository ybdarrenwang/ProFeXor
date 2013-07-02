#ifndef MEANSUBTRACTION_H
#define MEANSUBTRACTION_H

#include "Normalizer.h"

using namespace std;

class MeanSubtraction : public Normalizer
{
	public:
		void Normalize(vector<double>&, UtteranceInfo*);
};

#endif
