#ifndef MEANVARNORM_H
#define MEANVARNORM_H

#include "Normalizer.h"

using namespace std;

/**
* Mean Variance Normalization (MVN)
*/
class MeanVarNorm : public Normalizer
{
	public:
		void Normalize(vector<double>&, UtteranceInfo*);
};

#endif
