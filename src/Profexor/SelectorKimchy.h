#ifndef SELECTORKIMCHY_H
#define SELECTORKIMCHY_H

#include "FeatureSelector.h"
#include "PitchFeXor.h"
#include "SRIPitchFeXor.h"
#include "DurationFeXor.h"
#include "EnergyFeXor.h"
#include "PauseFeXor.h"

/**
* This class is the feature set defined by Che-Kuang Lin\n
* Reference: Che-Kuang Lin, "New Appraoches for Detecting Edit Disfluencies in Transcribing Spontaneous Mandarin Speech," Ph.D. thesis, National Taiwan University, 2009.
*/
class SelectorKimchy : public FeatureSelector
{
	public:
		SelectorKimchy();
		~SelectorKimchy(){}
		void GetExtractors(vector<FeatureExtractor*>&);
};

#endif
