#ifndef UTTERANCE_H
#define UTTERANCE_H

#include "AcousticUnit.h"
#include "Syllable.h"
#include "Pause.h"
#include "mymath.h"

class Utterance : public AcousticUnit
{
	public:
//		Utterance(UtteranceInfo*, Config*, ContourSmoother* pitchContourSmoother, ContourInterpolater* pitchContourInterpolater, Normalizer* pitchContourNormalizer, Normalizer* energyContourNormalizer);
		Utterance(){}
		Utterance(const Utterance&);
		~Utterance();
		int GetNumberOfSyllables() const {return (int)syllables.size();}
		Syllable* GetSyllable(int i) const {return syllables[i];}
		void SetPitchContour(vector<double> c);
		void SetEnergyContour(vector<double> c);
		void AddSyllable(Syllable* s) {syllables.push_back(s);}

	private:
		vector<Syllable*> syllables;
};

#endif
