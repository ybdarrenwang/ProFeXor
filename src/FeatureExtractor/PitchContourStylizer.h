#ifndef PITCHCONTOURSTYLIZER_H
#define PITCHCONTOURSTYLIZER_H

#include "Utterance.h"

class PitchContourStylizer
{
	public:
		virtual ~PitchContourStylizer(){}
		virtual void Stylize(Utterance*) = 0;
};

#endif
