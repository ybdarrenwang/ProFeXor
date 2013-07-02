#include "DurationFeXor.h"

void DurationFeXor::InitializeFeature(int numSyllable)
{
	cout<<"Initializing features"<<endl;

	sylDur = vector<double>(numSyllable, 0);
	n_sylDur = vector<double>(numSyllable, 0);
	avgWithPrevSylDur = vector<double>(numSyllable, 0);
	n_avgWithPrevSylDur = vector<double>(numSyllable, 0);
	ratioSylDur_SylBesideBoundary = vector<double>(numSyllable, 0);
	ratioSylDur_OneSylApartFromBoundary = vector<double>(numSyllable, 0);
	ratioSylDur_TwoSylApartFromBoundary = vector<double>(numSyllable, 0);
	ratioAvgSylDurAcrossBoundary = vector<double>(numSyllable, 0);
	stdDev_sylDur = vector<double>(numSyllable, 0);
	onsetDur = vector<double>(numSyllable, 0);
	n_onsetDur = vector<double>(numSyllable, 0);
	nucleiDur = vector<double>(numSyllable, 0);
	n_nucleiDur = vector<double>(numSyllable, 0);
	codaDur = vector<double>(numSyllable, 0);
	n_codaDur = vector<double>(numSyllable, 0);
}

void DurationFeXor::InitializeDeltaFeature(int numSyllable)
{
	cout<<"Initializing delta features"<<endl;

	d_sylDur = vector<double>(numSyllable, 0);
	d_n_sylDur = vector<double>(numSyllable, 0);
	d_avgWithPrevSylDur = vector<double>(numSyllable, 0);
	d_n_avgWithPrevSylDur = vector<double>(numSyllable, 0);
	d_ratioSylDur_SylBesideBoundary = vector<double>(numSyllable, 0);
	d_ratioSylDur_OneSylApartFromBoundary = vector<double>(numSyllable, 0);
	d_ratioSylDur_TwoSylApartFromBoundary = vector<double>(numSyllable, 0);
	d_ratioAvgSylDurAcrossBoundary = vector<double>(numSyllable, 0);
	d_stdDev_sylDur = vector<double>(numSyllable, 0);
}

void DurationFeXor::GetIntraSyllableFeature(Utterance* u)
{
	cout<<"Get intra-syllable duration features"<<endl;

	avgSylDuration = 0;
	for (int s=0; s<u->GetNumberOfSyllables(); s++)
		avgSylDuration+=u->GetSyllable(s)->GetDuration();
	avgSylDuration/=u->GetNumberOfSyllables();

	for (int s=0; s<u->GetNumberOfSyllables(); s++)
	{
		Syllable* syl = u->GetSyllable(s);
		sylDur[s] = syl->GetDuration();
		n_sylDur[s] = sylDur[s]/avgSylDuration;
		if (s>0)
		{
			avgWithPrevSylDur[s] =  (sylDur[s]+sylDur[s-1])/2;
			n_avgWithPrevSylDur[s] =  (n_sylDur[s] + n_sylDur[s-1]) / 2;
		}

		onsetDur[s] = syl->GetOnset()->GetDuration();
		n_onsetDur[s] = onsetDur[s]/avgSylDuration;

		nucleiDur[s] = syl->GetNuclei()->GetDuration();
		n_nucleiDur[s] = nucleiDur[s]/avgSylDuration;

		codaDur[s] = syl->GetCoda()->GetDuration();
		n_codaDur[s] = codaDur[s]/avgSylDuration;
	}
}	

void DurationFeXor::GetCrossSyllableFeature(int numSyllable)
{
	cout<<"Get cross-syllable duration features"<<endl;

	int s2s_fw_counter = numSyllable; // number of syllables from the begin of sentence to here
	int s2s_bw_counter = 0;
	double s2s_fw_dur_sum = avgSylDuration*numSyllable; // sum of syllable duration before this syllable
	double s2s_bw_dur_sum = 0; // sum of syllable duration after this syllable
	for (int s=0; s<numSyllable; s++)
	{
		// syllable duration ratio, calculated near syllable boundaries
		if (s>0) ratioSylDur_SylBesideBoundary[s] = sylDur[s]/sylDur[s-1];
		if (s>1&s<numSyllable-1) ratioSylDur_OneSylApartFromBoundary[s] = sylDur[s+1]/sylDur[s-2];
		if (s>2&s<numSyllable-2) ratioSylDur_TwoSylApartFromBoundary[s] = sylDur[s+2]/sylDur[s-3];

		// average syllable duration ratio, calculate between sentence boundaries
		if (s2s_bw_counter && s2s_fw_counter)
			ratioAvgSylDurAcrossBoundary[s] = (s2s_bw_dur_sum/s2s_bw_counter)/(s2s_fw_dur_sum/s2s_fw_counter);

		s2s_bw_dur_sum += sylDur[s];
		s2s_fw_dur_sum -= sylDur[s];
		s2s_bw_counter++;
		s2s_fw_counter--;

		// calculate standard deviation of the current and previous 2 syllable durations
		if (s >= 2)
		{
			vector<double> tmp_3sylDurs = subVec(sylDur, s-2, s);
			stdDev_sylDur[s] = getSD(tmp_3sylDurs);
		}
	}
}

void DurationFeXor::GetDeltaFeature(int numSyllable)
{
	cout<<"Get duration delta-features"<<endl;

	for (int s=1; s<numSyllable; s++)
	{
		d_sylDur[s] =  sylDur[s] - sylDur[s-1];
		d_n_sylDur[s] =  n_sylDur[s] - n_sylDur[s-1];
		d_avgWithPrevSylDur[s] =  avgWithPrevSylDur[s] - avgWithPrevSylDur[s-1];
		d_n_avgWithPrevSylDur[s] =  n_avgWithPrevSylDur[s] - n_avgWithPrevSylDur[s-1];
		d_ratioSylDur_SylBesideBoundary[s] = ratioSylDur_SylBesideBoundary[s] - ratioSylDur_SylBesideBoundary[s-1];
		d_ratioSylDur_OneSylApartFromBoundary[s] = ratioSylDur_OneSylApartFromBoundary[s] - ratioSylDur_OneSylApartFromBoundary[s-1];
		d_ratioSylDur_TwoSylApartFromBoundary[s] = ratioSylDur_TwoSylApartFromBoundary[s] - ratioSylDur_TwoSylApartFromBoundary[s-1];
		d_ratioAvgSylDurAcrossBoundary[s] = ratioAvgSylDurAcrossBoundary[s] - ratioAvgSylDurAcrossBoundary[s-1];
		d_stdDev_sylDur[s] = stdDev_sylDur[s] - stdDev_sylDur[s-1];
	}
}

void DurationFeXor::SaveFeature(vector< vector<double> >& features, int numSyllable)
{
	cout<<"Saving duration features"<<endl;

	for (int s=0; s<numSyllable; s++)
	{
		features[s].push_back(sylDur[s]);
		features[s].push_back(n_sylDur[s]);
		features[s].push_back(avgWithPrevSylDur[s]);
		features[s].push_back(n_avgWithPrevSylDur[s]);
		features[s].push_back(ratioSylDur_SylBesideBoundary[s]);
		features[s].push_back(ratioSylDur_OneSylApartFromBoundary[s]);
		features[s].push_back(ratioSylDur_TwoSylApartFromBoundary[s]);
		features[s].push_back(ratioAvgSylDurAcrossBoundary[s]);
		features[s].push_back(stdDev_sylDur[s]);
		features[s].push_back(onsetDur[s]);
		features[s].push_back(n_onsetDur[s]);
		features[s].push_back(nucleiDur[s]);
		features[s].push_back(n_nucleiDur[s]);
		features[s].push_back(codaDur[s]);
		features[s].push_back(n_codaDur[s]);
	}
}

void DurationFeXor::SaveDeltaFeature(vector< vector<double> >& features, int numSyllable)
{
	cout<<"Saving duration delta-features"<<endl;

	for (int s=0; s<numSyllable; s++)
	{
		features[s].push_back(d_sylDur[s]);
		features[s].push_back(d_n_sylDur[s]);
		features[s].push_back(d_avgWithPrevSylDur[s]);
		features[s].push_back(d_n_avgWithPrevSylDur[s]);
		features[s].push_back(d_ratioSylDur_SylBesideBoundary[s]);
		features[s].push_back(d_ratioSylDur_OneSylApartFromBoundary[s]);
		features[s].push_back(d_ratioSylDur_TwoSylApartFromBoundary[s]);
		features[s].push_back(d_ratioAvgSylDurAcrossBoundary[s]);
		features[s].push_back(d_stdDev_sylDur[s]);
	}
}

void DurationFeXor::ResetFeature()
{
	cout<<"Reset DurationFeXor cache"<<endl;

	sylDur.clear();
	n_sylDur.clear();
	avgWithPrevSylDur.clear();
	n_avgWithPrevSylDur.clear();
	ratioSylDur_SylBesideBoundary.clear();
	ratioSylDur_OneSylApartFromBoundary.clear();
	ratioSylDur_TwoSylApartFromBoundary.clear();
	ratioAvgSylDurAcrossBoundary.clear();
	stdDev_sylDur.clear();
	onsetDur.clear();
	n_onsetDur.clear();
	nucleiDur.clear();
	n_nucleiDur.clear();
	codaDur.clear();
	n_codaDur.clear();
}

void DurationFeXor::ResetDeltaFeature()
{
	cout<<"Reset DurationFeXor cache"<<endl;

	d_sylDur.clear();
	d_n_sylDur.clear();
	d_avgWithPrevSylDur.clear();
	d_n_avgWithPrevSylDur.clear();
	d_ratioSylDur_SylBesideBoundary.clear();
	d_ratioSylDur_OneSylApartFromBoundary.clear();
	d_ratioSylDur_TwoSylApartFromBoundary.clear();
	d_ratioAvgSylDurAcrossBoundary.clear();
	d_stdDev_sylDur.clear();
}

/**
 *0.Initialize feature vectors as zero vectors\n
 *1.Extract intra-syllable features\n
 *2.Extract cross-syllable features\n
 *3.Save features
 */
void DurationFeXor::Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta)
{
	cout<<"=== Extracting duration features ==="<<endl;
	InitializeFeature(u->GetNumberOfSyllables());
	GetIntraSyllableFeature(u);
	GetCrossSyllableFeature(u->GetNumberOfSyllables());
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

void DurationFeXor::PrintDefinition(vector<string>& definition)
{
	definition.push_back("<Duration::sylDur> Syllable duration");
	definition.push_back("<Duration::n_sylDur> Syllable duration, normalized with sentence average syllable duration");
	definition.push_back("<Duration::avgWithPrevSylDur> Average duration of current and preceding syllable");
	definition.push_back("<Duration::n_avgWithPrevSylDur> Average duration of current and preceding syllable, normalized with sentence average syllable duration");
	definition.push_back("<Duration::ratioSylDur_SylBesideBoundary> Ratio between the durations of 2 syllables beside this boundary");
	definition.push_back("<Duration::ratioSylDur_OneSylApartFromBoundary> Ratio between the durations of 2 syllables, which are 1 syllable apart from this boundary");
	definition.push_back("<Duration::ratioSylDur_TwoSylApartFromBoundary> Ratio between the durations of 2 syllables, which are 2 syllables apart from this boundary");
	definition.push_back("<Duration::ratioAvgSylDurAcrossBoundary> The ratio of average durations of the syllables before and after a break, calculated from sentence boundary");
	definition.push_back("<Duration::stdDev_sylDur> Standard deviation of current and 2 preceding syllable durations");
	definition.push_back("<Duration::onsetDur> Initial (onset) duration");
	definition.push_back("<Duration::n_onsetDur> Initial (onset) duration, normalized with sentence average syllable duration");
	definition.push_back("<Duration::nucleiDur> Final (nuclei) duration");
	definition.push_back("<Duration::n_nucleiDur> Final (nuclei) duration, normalized with sentence average syllable duration");
	definition.push_back("<Duration::codaDur> Coda duration");
	definition.push_back("<Duration::n_codaDur> Coda duration, normalized with sentence average syllable duration");
}

void DurationFeXor::PrintDeltaDefinition(vector<string>& definition)
{
	definition.push_back("<Duration::d_sylDur> Delta, syllable duration");
	definition.push_back("<Duration::d_n_sylDur> Delta, syllable duration, normalized with sentence average syllable duration");
	definition.push_back("<Duration::d_avgWithPrevSylDur> Delta, average duration of current and preceding syllable");
	definition.push_back("<Duration::d_n_avgWithPrevSylDur> Delta, average duration of current and preceding syllable, normalized with sentence average syllable duration");
	definition.push_back("<Duration::d_ratioSylDur_SylBesideBoundary> Delta, ratio between the durations of 2 syllables beside this boundary");
	definition.push_back("<Duration::d_ratioSylDur_OneSylApartFromBoundary> Delta, ratio between the durations of 2 syllables, which are 1 syllable apart from this boundary");
	definition.push_back("<Duration::d_ratioSylDur_TwoSylApartFromBoundary> Delta, ratio between the durations of 2 syllables, which are 2 syllables apart from this boundary");
	definition.push_back("<Duration::d_ratioAvgSylDurAcrossBoundary> Delta, the ratio of average durations of the syllables before and after a break, calculated from sentence boundary");
	definition.push_back("<Duration::d_stdDev_sylDur> Delta, standard deviation of current and 2 preceding syllable durations");
}
