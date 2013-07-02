#ifndef MOVINGWINDOWMEANSUB
#define MOVINGWINDOWMEANSUB

#include "Normalizer.h"

using namespace std;

class MovingWindowMeanSub : public Normalizer
{
	public:
		MovingWindowMeanSub(int s){windowSize = s;}
		void Normalize(vector<double>&, UtteranceInfo*);

	private:
		int windowSize;
};

#endif
