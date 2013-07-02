#include "PauseFeXor.h"
#include "Syllable.h"
#include "Pause.h"
#include "mymath.h"

using namespace std;

void PauseFeXor::InitializeFeature(int numSyllable)
{
	cout<<"Initializing features"<<endl;
	ratioAvgSylDurationFromPrevUntilNextPause = vector<double>(numSyllable, 0);
	ratio_prevSyl_avgSylDurationFromPrevPause = vector<double>(numSyllable, 0);
	mulPauseSylDuration = vector<double>(numSyllable, 0);
	mulSylPauseDuration = vector<double>(numSyllable, 0);
	ratioPauseSylDuration = vector<double>(numSyllable, 0);
	ratioSylPauseDuration = vector<double>(numSyllable, 0);
	stdDev_mulPauseSylDuration = vector<double>(numSyllable, 0);
	stdDev_mulSylPauseDuration = vector<double>(numSyllable, 0);
	stdDev_ratioPauseSylDuration = vector<double>(numSyllable, 0);
	stdDev_ratioSylPauseDuration = vector<double>(numSyllable, 0);
	sumSylDurationFromPrevPause = vector<double>(numSyllable, 0);
	numSylFromPrevPause = vector<double>(numSyllable, 0);
}

void PauseFeXor::InitializeDeltaFeature(int numSyllable)
{
	cout<<"Initializing delta-features"<<endl;
	d_ratioAvgSylDurationFromPrevUntilNextPause = vector<double>(numSyllable, 0);
	d_ratio_prevSyl_avgSylDurationFromPrevPause = vector<double>(numSyllable, 0);
	d_mulPauseSylDuration = vector<double>(numSyllable, 0);
	d_mulSylPauseDuration = vector<double>(numSyllable, 0);
	d_ratioPauseSylDuration = vector<double>(numSyllable, 0);
	d_ratioSylPauseDuration = vector<double>(numSyllable, 0);
	d_stdDev_mulPauseSylDuration = vector<double>(numSyllable, 0);
	d_stdDev_mulSylPauseDuration = vector<double>(numSyllable, 0);
	d_stdDev_ratioPauseSylDuration = vector<double>(numSyllable, 0);
	d_stdDev_ratioSylPauseDuration = vector<double>(numSyllable, 0);
}

void PauseFeXor::GetFeature(Utterance* u)
{
	cout<<"Get pause features"<<endl;

	Syllable *syllable, *prevSyllable, *nextSyllable;
	Pause* prevPause;
	int tmpNumSylFromPrevPause = 0;
	int tmpNumSylUntilNextPause = 0;
	double tmpSumSylDurationFromPrevPause = 0;
	double tmpSumSylDurationUntilNextPause = 0;

	for(int s=0; s<u->GetNumberOfSyllables(); s++)
	{
		syllable = u->GetSyllable(s);
		prevSyllable = syllable->GetPrevSyllable();
		nextSyllable = syllable->GetNextSyllable();
		prevPause = syllable->GetPrevPause();

		// gather required syllable and pause duration information
		if (prevPause != 0 || prevSyllable == 0)
		{
			tmpNumSylUntilNextPause = 1;
			tmpSumSylDurationUntilNextPause = syllable->GetDuration();
			Syllable* tmpSyllable = syllable;
			Syllable* tmpNextSyllable = nextSyllable;
			while(tmpNextSyllable!=0)
			{
				if (tmpNextSyllable->GetPrevPause()==0)
				{
					tmpSyllable = tmpSyllable->GetNextSyllable();

					tmpNumSylUntilNextPause++;
					tmpSumSylDurationUntilNextPause += tmpSyllable->GetDuration();

					tmpNextSyllable = tmpNextSyllable->GetNextSyllable();
				}
				else
					break;
			}
			tmpNumSylFromPrevPause = 0;
			tmpSumSylDurationFromPrevPause = 0;
		}
		else
		{
			tmpSumSylDurationFromPrevPause += syllable->GetDuration();
			tmpSumSylDurationUntilNextPause -= syllable->GetDuration();
			tmpNumSylFromPrevPause++;
			tmpNumSylUntilNextPause--;

		}

		// long-term features
		if (tmpNumSylFromPrevPause)
		{
			ratio_prevSyl_avgSylDurationFromPrevPause[s] = prevSyllable->GetDuration()/(tmpSumSylDurationFromPrevPause/tmpNumSylFromPrevPause);
			if (tmpNumSylUntilNextPause)
				ratioAvgSylDurationFromPrevUntilNextPause[s] = (tmpSumSylDurationFromPrevPause/tmpNumSylFromPrevPause)/(tmpSumSylDurationUntilNextPause/tmpNumSylUntilNextPause);
		}
		sumSylDurationFromPrevPause[s] = tmpSumSylDurationFromPrevPause;
		numSylFromPrevPause[s] = tmpNumSylFromPrevPause;

		// the product or ratio between syllable and pause durations
		if (prevPause != 0)
		{
			mulPauseSylDuration[s] = syllable->GetDuration() * prevPause->GetDuration();
			ratioPauseSylDuration[s] = syllable->GetDuration() / prevPause->GetDuration();

			if (prevSyllable != 0)
			{
				mulSylPauseDuration[s] = prevSyllable->GetDuration() * prevPause->GetDuration();
				ratioSylPauseDuration[s] = prevSyllable->GetDuration() / prevPause->GetDuration();
			}
		}

		// the standard deviation of the product or ratio between syllable and pause durations
		if(s>=2)
		{
			vector<double> tmpVector;
			tmpVector = subVec(mulPauseSylDuration, s-2, s);
			stdDev_mulPauseSylDuration[s] = getSD(tmpVector);
			tmpVector = subVec(ratioPauseSylDuration, s-2, s);
			stdDev_ratioPauseSylDuration[s] = getSD(tmpVector);
			tmpVector = subVec(mulSylPauseDuration, s-2, s);
			stdDev_mulSylPauseDuration[s] = getSD(tmpVector);
			tmpVector = subVec(ratioSylPauseDuration, s-2, s);
			stdDev_ratioSylPauseDuration[s] = getSD(tmpVector);
		}
	}
}

void PauseFeXor::GetDeltaFeature(int numSyllable)
{
	cout<<"Get pause delta-features"<<endl;

	for (int s=1; s<numSyllable; s++)
	{
		d_ratioAvgSylDurationFromPrevUntilNextPause[s] = ratioAvgSylDurationFromPrevUntilNextPause[s] - ratioAvgSylDurationFromPrevUntilNextPause[s-1];
		d_ratio_prevSyl_avgSylDurationFromPrevPause[s] = ratio_prevSyl_avgSylDurationFromPrevPause[s] - ratio_prevSyl_avgSylDurationFromPrevPause[s-1];
		d_mulPauseSylDuration[s] = mulPauseSylDuration[s] - mulPauseSylDuration[s-1];
		d_mulSylPauseDuration[s] = mulSylPauseDuration[s] - mulSylPauseDuration[s-1];
		d_ratioPauseSylDuration[s] = ratioPauseSylDuration[s] - ratioPauseSylDuration[s-1];
		d_ratioSylPauseDuration[s] = ratioSylPauseDuration[s] - ratioSylPauseDuration[s-1];
		d_stdDev_mulPauseSylDuration[s] = stdDev_mulPauseSylDuration[s] - stdDev_mulPauseSylDuration[s-1];
		d_stdDev_mulSylPauseDuration[s] = stdDev_mulSylPauseDuration[s] - stdDev_mulSylPauseDuration[s-1];
		d_stdDev_ratioPauseSylDuration[s] = stdDev_ratioPauseSylDuration[s] - stdDev_ratioPauseSylDuration[s-1];
		d_stdDev_ratioSylPauseDuration[s] = stdDev_ratioSylPauseDuration[s] - stdDev_ratioSylPauseDuration[s-1];
	}
}


void PauseFeXor::SaveFeature(vector< vector<double> >& features, int numSyllable)
{
	cout<<"Saving pause features"<<endl;

	for (int s=0; s<numSyllable; s++)
	{
		features[s].push_back(ratioAvgSylDurationFromPrevUntilNextPause[s]);
		features[s].push_back(ratio_prevSyl_avgSylDurationFromPrevPause[s]);
		features[s].push_back(mulPauseSylDuration[s]);
		features[s].push_back(mulSylPauseDuration[s]);
		features[s].push_back(ratioPauseSylDuration[s]);
		features[s].push_back(ratioSylPauseDuration[s]);
		features[s].push_back(stdDev_mulPauseSylDuration[s]);
		features[s].push_back(stdDev_mulSylPauseDuration[s]);
		features[s].push_back(stdDev_ratioPauseSylDuration[s]);
		features[s].push_back(stdDev_ratioSylPauseDuration[s]);
		features[s].push_back(sumSylDurationFromPrevPause[s]);
		features[s].push_back(numSylFromPrevPause[s]);
	}
}

void PauseFeXor::SaveDeltaFeature(vector< vector<double> >& features, int numSyllable)
{
	cout<<"Saving pause delta-features"<<endl;

	for (int s=0; s<numSyllable; s++)
	{
		features[s].push_back(d_ratioAvgSylDurationFromPrevUntilNextPause[s]);
		features[s].push_back(d_ratio_prevSyl_avgSylDurationFromPrevPause[s]);
		features[s].push_back(d_mulPauseSylDuration[s]);
		features[s].push_back(d_mulSylPauseDuration[s]);
		features[s].push_back(d_ratioPauseSylDuration[s]);
		features[s].push_back(d_ratioSylPauseDuration[s]);
		features[s].push_back(d_stdDev_mulPauseSylDuration[s]);
		features[s].push_back(d_stdDev_mulSylPauseDuration[s]);
		features[s].push_back(d_stdDev_ratioPauseSylDuration[s]);
		features[s].push_back(d_stdDev_ratioSylPauseDuration[s]);
	}
}

void PauseFeXor::ResetFeature()
{
	cout<<"Reset PauseFeXor cache"<<endl;

	ratioAvgSylDurationFromPrevUntilNextPause.clear();
	ratio_prevSyl_avgSylDurationFromPrevPause.clear();
	mulPauseSylDuration.clear();
	mulSylPauseDuration.clear();
	ratioPauseSylDuration.clear();
	ratioSylPauseDuration.clear();
	stdDev_mulPauseSylDuration.clear();
	stdDev_mulSylPauseDuration.clear();
	stdDev_ratioPauseSylDuration.clear();
	stdDev_ratioSylPauseDuration.clear();
	sumSylDurationFromPrevPause.clear();
	numSylFromPrevPause.clear();
}

void PauseFeXor::ResetDeltaFeature()
{
	cout<<"Reset PauseFeXor cache"<<endl;
	d_ratioAvgSylDurationFromPrevUntilNextPause.clear();
	d_ratio_prevSyl_avgSylDurationFromPrevPause.clear();
	d_mulPauseSylDuration.clear();
	d_mulSylPauseDuration.clear();
	d_ratioPauseSylDuration.clear();
	d_ratioSylPauseDuration.clear();
	d_stdDev_mulPauseSylDuration.clear();
	d_stdDev_mulSylPauseDuration.clear();
	d_stdDev_ratioPauseSylDuration.clear();
	d_stdDev_ratioSylPauseDuration.clear();
}

/**
 *0.Initialize feature vectors as zero vectors\n
 *1.Extract intra-syllable features\n
 *2.Extract cross-syllable features\n
 *3.Save features
 */
void PauseFeXor::Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta)
{
	cout<<"=== Extracting pause features ==="<<endl;
	InitializeFeature(u->GetNumberOfSyllables());
	GetFeature(u);
	SaveFeature(features, u->GetNumberOfSyllables());

	if (extractDelta)
	{
		InitializeDeltaFeature(u->GetNumberOfSyllables());
		GetDeltaFeature(u->GetNumberOfSyllables());
		SaveDeltaFeature(features, u->GetNumberOfSyllables());
		ResetDeltaFeature();
	}

	ResetFeature();
}

void PauseFeXor::PrintDefinition(vector<string>& definition)
{
	definition.push_back("<Pause::ratioAvgSylDurationFromPrevUntilNextPause> The ratio of average durations of the syllables before and after this break, calculated from the nearest short pause");
	definition.push_back("<Pause::ratio_prevSyl_avgSylDurationFromPrevPause> The syllable duration before this boundary, divided by average syllable duration calculted from previous pause to this boundary");
	definition.push_back("<Pause::mulPauseSylDuration> Product between pause duration and the following syllable duration");
	definition.push_back("<Pause::mulSylPauseDuration> Product between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::ratioPauseSylDuration> Ratio between pause duration and the following syllable duration");
	definition.push_back("<Pause::ratioSylPauseDuration> Ratio between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::stdDev_mulPauseSylDuration> Standard deviation of current and 2 preceding values of the product between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::stdDev_mulSylPauseDuration> Standard deviation of current and 2 preceding values of the product between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::stdDev_ratioPauseSylDuration> Standard deviation of current and 2 preceding values of the ratio between pause duration and the following syllable duration");
	definition.push_back("<Pause::stdDev_ratioSylPauseDuration> Standard deviation of current and 2 preceding values of the ratio between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::sumSylDurationFromPrevPause> Number of frames from the previous pause to here");
	definition.push_back("<Pause::numSylFromPrevPause> Number of syllables from the previous pause to here");
}

void PauseFeXor::PrintDeltaDefinition(vector<string>& definition)
{
	definition.push_back("<Pause::d_ratioAvgSylDurationFromPrevUntilNextPause> Delta, the ratio of average durations of the syllables before and after this break, calculated from the nearest short pause");
	definition.push_back("<Pause::d_ratio_prevSyl_avgSylDurationFromPrevPause> Delta, the syllable duration before this boundary, divided by average syllable duration calculted from previous pause to this boundary");
	definition.push_back("<Pause::d_mulPauseSylDuration> Delta, product between pause duration and the following syllable duration");
	definition.push_back("<Pause::d_mulSylPauseDuration> Delta, product between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::d_ratioPauseSylDuration> Delta, ratio between pause duration and the following syllable duration");
	definition.push_back("<Pause::d_ratioSylPauseDuration> Delta, ratio between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::d_stdDev_mulPauseSylDuration> Delta, standard deviation of current and 2 preceding values of the product between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::d_stdDev_mulSylPauseDuration> Delta, standard deviation of current and 2 preceding values of the product between pause duration and the preceding syllable duration");
	definition.push_back("<Pause::d_stdDev_ratioPauseSylDuration> Delta, standard deviation of current and 2 preceding values of the ratio between pause duration and the following syllable duration");
	definition.push_back("<Pause::d_stdDev_ratioSylPauseDuration> Delta, standard deviation of current and 2 preceding values of the ratio between pause duration and the preceding syllable duration");
}
