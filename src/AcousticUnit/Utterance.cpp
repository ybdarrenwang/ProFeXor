#include "Utterance.h"

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
