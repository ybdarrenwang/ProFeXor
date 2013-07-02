#include "Utterance.h"

/**
*1.Load energy contour from wav file, and perform normalization.\n
*2.Load pitch contour from f0 file, take log, and perform normalization, interpolation and smoothing.\n
*3.Create units based on time labels
*/
/*Utterance::Utterance(UtteranceInfo* info, Config* config, ContourSmoother* pitchContourSmoother, ContourInterpolater* pitchContourInterpolater, Normalizer* pitchContourNormalizer, Normalizer* energyContourNormalizer)
{
	// Load energy contour from wav file
	LoadWave(info->GetWaveFile(), config->GetNoHeader(), config->GetSamplingRate(), energyContour);
	if (energyContourNormalizer != 0) energyContourNormalizer->Normalize(energyContour);

	// Load pitch contour from f0 file, and do interpolation and smoothing
	LoadF0(info->GetF0File(), pitchContour);
	Vector_log(pitchContour);
	if (pitchContourInterpolater != 0) pitchContourInterpolater->Interpolate(pitchContour);
	if (pitchContourSmoother != 0) pitchContourSmoother->Smooth(pitchContour);
	if (pitchContourNormalizer != 0) pitchContourNormalizer->Normalize(pitchContour);

	// Create units based on time labels
	vector< vector<int> > rawTimeLabel = info->GetTimeLabel();
	Syllable* prevSyllable = 0;
	Pause* prevPause = 0;
	for (int row=0; row<rawTimeLabel.size(); row++)
	{
		if (rawTimeLabel[row][1] == -1 & rawTimeLabel[row][2] == -1) // pause
		{
			prevPause = new Pause(rawTimeLabel[row][0], rawTimeLabel[row][3]);
		}
		else // syllable
		{
			Syllable* syllable = new Syllable(rawTimeLabel[row][0], rawTimeLabel[row][1], rawTimeLabel[row][2], rawTimeLabel[row][3]);
			syllable->SetPrevSyllable(prevSyllable);
			syllable->SetPrevPause(prevPause);
			syllable->SetPitchContour(subVec(pitchContour, rawTimeLabel[row][0], rawTimeLabel[row][3]-1));
			syllable->SetEnergyContour(subVec(energyContour, rawTimeLabel[row][0], rawTimeLabel[row][3]-1));
			syllables.push_back(syllable);
			if (prevSyllable != 0) prevSyllable->SetNextSyllable(syllables[syllables.size()-1]);
			prevSyllable = syllables[syllables.size()-1];
			prevPause = 0;
		}
	}
}*/

Utterance::Utterance(const Utterance& u)
{
	beginTime = u.GetBeginTime();
	endTime = u.GetEndTime();
	for (int i=0; i<u.GetNumberOfSyllables(); i++)
		syllables.push_back(u.GetSyllable(i));
	SetPitchContour(u.GetPitchContour());
	SetEnergyContour(u.GetEnergyContour());
}

Utterance::~Utterance()
{
	for (vector<Syllable*>::iterator i=syllables.begin(); i!=syllables.end(); i++)
		delete (*i);
}

void Utterance::SetPitchContour(vector<double> c)
{
	pitchContour = c;
	for (int i=0; i<syllables.size(); i++)
		syllables[i]->SetPitchContour(subVec(c, syllables[i]->GetBeginTime(), syllables[i]->GetEndTime()-1));
}

void Utterance::SetEnergyContour(vector<double> c)
{
	energyContour = c;
	for (int i=0; i<syllables.size(); i++)
		syllables[i]->SetEnergyContour(subVec(c, syllables[i]->GetBeginTime(), syllables[i]->GetEndTime()-1));
}
