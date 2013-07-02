#include "Syllable.h"
#include "mymath.h"

void Syllable::SetPitchContour(vector<double> c)
{
	pitchContour = c;
	onset->SetPitchContour(subVec(c, 0, onset->GetDuration()-1));
	nuclei->SetPitchContour(subVec(c, onset->GetDuration(), onset->GetDuration()+nuclei->GetDuration()-1));
	coda->SetPitchContour(subVec(c, onset->GetDuration()+nuclei->GetDuration(), this->GetDuration()-1));
}

void Syllable::SetEnergyContour(vector<double> c)
{
	energyContour = c;
	onset->SetEnergyContour(subVec(c, 0, onset->GetDuration()-1));
	nuclei->SetEnergyContour(subVec(c, onset->GetDuration(), onset->GetDuration()+nuclei->GetDuration()-1));
	coda->SetEnergyContour(subVec(c, onset->GetDuration()+nuclei->GetDuration(), this->GetDuration()-1));
}
