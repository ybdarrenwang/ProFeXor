#ifndef	UTTERANCEBUILDER
#define	UTTERANCEBUILDER

#include "Utterance.h"
#include "UtteranceInfo.h"
#include "AcousticUnit.h"
#include "Syllable.h"
#include "Pause.h"
#include "Normalizer.h"
#include "MeanSubtraction.h"
#include "MovingWindowMeanSub.h"
#include "MaxSubtraction.h"
#include "ContourSmoother.h"
#include "MovingWindowSmoother.h"
#include "ContourInterpolater.h"
#include "CubicSplineInterpolater.h"
#include "LinearInterpolater.h"
#include "Config.h"
#include "wavIO.h"
#include "util.h"
#include "mymath.h"

class UtteranceBuilder
{
	public:
		UtteranceBuilder(Config* c);
		~UtteranceBuilder();
		void NewUtterance();
		void BuildAcousticUnit(UtteranceInfo*);
		void BuildPitchContour(UtteranceInfo*);
		void BuildEnergyContour(UtteranceInfo*);
		Utterance* GetResult(){return utt;}

	protected:
		bool noHeader; ///< wave file
		int samplingRate; ///< wave file
		ContourSmoother* pitchContourSmoother;
		ContourInterpolater* pitchContourInterpolater;
		Normalizer* pitchContourNormalizer;
		Normalizer* energyContourNormalizer;
		vector<double> energyContour;
		vector<double> pitchContour;
		Utterance* utt; ///< final result
};

#endif
