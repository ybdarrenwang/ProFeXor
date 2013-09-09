#ifndef NORMALIZER
#define NORMALIZER

#include "global.h"
#include "UtteranceInfo.h"
#include "debug.h"

using namespace std;

/**
 * Note: In the derived classes of Normalizer, we generally make use
 * of the time labels given by UtteranceInfo in the Normalize()
 * function. More specifically:\n
 * 1. we assume the begin and end of utterances are silences;\n
 * 2. we exclude them while calculating statistics;\n
 * 3. while normalization, the beginning and end silences are
 *    directly set 0.
 */
class Normalizer
{
	public:
		virtual ~Normalizer(){}
		virtual void Normalize(vector<double>&, UtteranceInfo*) = 0;
};

#endif
