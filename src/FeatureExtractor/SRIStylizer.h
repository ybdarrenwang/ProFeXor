#ifndef SRISTYLIZER_H
#define SRISTYLIZER_H

#include "PitchContourStylizer.h"
#include "PiecewiseLinear.h"

/**
* SRI piecewise linear f0:\n
*   For each syllable, we segment and approximate the pitch contour into\n
*   N straight lines, and the best segmentation with MMSE is found.\n
*\n
* Reference: K. Sonmez, E. Shriberg, L. Heck, and M. Weintraub,\n
*            "Modeling dynamic prosodic variation for speaker verification,"\n
*            in Proc. ICSLP, 1998, pp. 3189~3192.
*/
class SRIStylizer : public PitchContourStylizer
{
	public:
		void Stylize(Utterance*);
};

#endif
