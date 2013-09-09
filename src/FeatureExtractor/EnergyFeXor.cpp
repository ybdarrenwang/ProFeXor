#include "EnergyFeXor.h"

void EnergyFeXor::InitializeFeature(int numOfSyllable)
{
	DUMP(__PRETTY_FUNCTION__);

	begin = vector<double>(numOfSyllable, 0);
	end = vector<double>(numOfSyllable, 0);
	slopeBegin = vector<double>(numOfSyllable, 0);
	slopeEnd = vector<double>(numOfSyllable, 0);
	min = vector<double>(numOfSyllable, 0);
	max = vector<double>(numOfSyllable, 0);
	avg = vector<double>(numOfSyllable, 0);
	avgSlope = vector<double>(numOfSyllable, 0);
	avgNuclei = vector<double>(numOfSyllable, 0);
	totalNuclei = vector<double>(numOfSyllable, 0);

	if (numOfSegment>0)
	{
		for (int s=0; s<numOfSyllable; s++)
			segment.push_back(vector<double>(numOfSegment, 0));
	}
}

void EnergyFeXor::InitializeDeltaFeature(int numOfSyllable)
{
	DUMP(__PRETTY_FUNCTION__);

	dBegin = vector<double>(numOfSyllable, 0);
	dEnd = vector<double>(numOfSyllable, 0);
	dbe = vector<double>(numOfSyllable, 0);
	ddbe = vector<double>(numOfSyllable, 0);
	dSlopeBegin = vector<double>(numOfSyllable, 0);
	dSlopeEnd = vector<double>(numOfSyllable, 0);
	dbeSlope = vector<double>(numOfSyllable, 0);
	ddbeSlope = vector<double>(numOfSyllable, 0);
	dMin = vector<double>(numOfSyllable, 0);
	dMax = vector<double>(numOfSyllable, 0);
	range = vector<double>(numOfSyllable, 0);
	dRange = vector<double>(numOfSyllable, 0);
	dAvg = vector<double>(numOfSyllable, 0);
	dAvgSlope = vector<double>(numOfSyllable, 0);
	dAvgNuclei = vector<double>(numOfSyllable, 0);
	if (numOfSegment>0)
	{
		for (int seg=0; seg<numOfSyllable; seg++)
		{
			dSyl_Segment.push_back(vector<double>(numOfSegment, 0));
			dSeg_Segment.push_back(vector<double>(numOfSegment-1, 0));
		}
	}
}

void EnergyFeXor::GetFeature(Utterance* u)
{
	DUMP(__PRETTY_FUNCTION__);

	Syllable* syl;
	vector<double> sylEnergyContour;
	vector<double> nucleiEnergyContour;
	for (int s=0; s<u->GetNumberOfSyllables(); s++)
	{
		syl = u->GetSyllable(s);
		sylEnergyContour = syl->GetEnergyContour();
		nucleiEnergyContour = syl->GetNuclei()->GetEnergyContour();
		int sylDuration = syl->GetDuration();
		int nucleiDuration = syl->GetNuclei()->GetDuration();

		// because energy contour has been subtracted with max energy,
		// only the minima of energy contour is informative
		getMinMax(&(sylEnergyContour[0]), min[s], max[s], sylDuration);

		// syllable boundary energy related feature
		begin[s] = sylEnergyContour[0];
		end[s] = sylEnergyContour[sylDuration-1];

		slopeBegin[s] = sylEnergyContour[0] - sylEnergyContour[1];
		slopeEnd[s] = sylEnergyContour[sylDuration-2] - sylEnergyContour[sylDuration-1];

		// syllable average feature
		avg[s] = getMean(&(sylEnergyContour[0]),sylDuration);

		for (int i=1; i<sylDuration; i++)
			avgSlope[s] += abs(sylEnergyContour[i] - sylEnergyContour[i-1]);
		avgSlope[s]/=(sylDuration-1);

		// nuclei energy related feature
		avgNuclei[s] = getMean(&(nucleiEnergyContour[0]), nucleiDuration);
		totalNuclei[s] = avgNuclei[s]*nucleiDuration;

		if (numOfSegment>0)
		{
			if(nucleiDuration < 3)
			{
				DUMP("[Warning] duration of nuclei is too short to break into segments, will calculate the features of whole contour as of each contour-segment");
				for (int seg=0; seg<numOfSegment; seg++)
					segment[s][seg] = avg[s];
			}
			else
			{
				int segBeginTime = 0;
				int segEndTime = (int)floor((double)(nucleiDuration)/numOfSegment)-1;
				for (int seg=0; seg<numOfSegment; seg++)
				{
					vector<double> segmentEnergyContour = subVec(nucleiEnergyContour, segBeginTime, segEndTime);
					segment[s][seg] = getMean(segmentEnergyContour);
					segBeginTime = segEndTime+1;
					segEndTime = (int)floor((seg+2)*((double)(nucleiDuration)/numOfSegment))-1;
				}
			}
		}

		sylEnergyContour.clear();
		nucleiEnergyContour.clear();
	}
}

void EnergyFeXor::GetDeltaFeature(int numOfSyllable)
{
	DUMP(__PRETTY_FUNCTION__);

	for (int s=0; s<numOfSyllable; s++)
	{
		dbe[s] = begin[s] - end[s];
		dbeSlope[s] = slopeBegin[s] - slopeEnd[s];
		range[s] = max[s] - min[s];

		if (numOfSegment>0)
		{
			for (int seg=0; seg<numOfSegment-1; seg++)
				dSeg_Segment[s][seg] = segment[s][seg+1] - segment[s][seg];
		}

		if (s!=0)
		{
			// syllable boundary energy related feature
			dBegin[s] = begin[s]-begin[s-1] ;
			dEnd[s] = end[s]-end[s-1];
			ddbe[s] = dbe[s]-dbe[s-1] ;
			dSlopeBegin[s] = slopeBegin[s]-slopeBegin[s-1];
			dSlopeEnd[s] = slopeEnd[s]-slopeEnd[s-1];
			ddbeSlope[s] = dbeSlope[s]-dbeSlope[s-1];

			// min. & max. energy related feature
			dMin[s] = min[s] - min[s-1];
			dMax[s] = max[s] - max[s-1];
			dRange[s] = range[s] - range[s-1];

			// syllable average feature
			dAvg[s] =  avg[s] - avg[s-1] ;
			dAvgSlope[s] =  avgSlope[s] - avgSlope[s-1] ;

			// nuclei energy related feature
			dAvgNuclei[s] =  avgNuclei[s] - avgNuclei[s-1] ;

			if (numOfSegment>0)
			{
				for (int seg=0; seg<numOfSegment; seg++)
					dSyl_Segment[s][seg] = segment[s][seg] - segment[s-1][seg];
			}
		}
	}
}

void EnergyFeXor::SaveFeature(vector< vector<double> >& features, int numOfSyllable)
{
	DUMP(__PRETTY_FUNCTION__);

	for (int s=0; s<numOfSyllable; s++)
	{
		features[s].push_back(begin[s]);
		features[s].push_back(end[s]);
		features[s].push_back(slopeBegin[s]);
		features[s].push_back(slopeEnd[s]);
		features[s].push_back(min[s]);
		features[s].push_back(max[s]);
		features[s].push_back(avg[s]);
		features[s].push_back(avgSlope[s]);
		features[s].push_back(avgNuclei[s]);
		features[s].push_back(totalNuclei[s]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(segment[s][seg]);
	}
}

void EnergyFeXor::SaveDeltaFeature(vector< vector<double> >& features, int numOfSyllable)
{
	DUMP(__PRETTY_FUNCTION__);

	for (int s=0; s<numOfSyllable; s++)
	{
		features[s].push_back(dBegin[s]);
		features[s].push_back(dEnd[s]);
		features[s].push_back(dbe[s]);
		features[s].push_back(ddbe[s]);
		features[s].push_back(dSlopeBegin[s]);
		features[s].push_back(dSlopeEnd[s]);
		features[s].push_back(dbeSlope[s]);
		features[s].push_back(ddbeSlope[s]);
		features[s].push_back(dMin[s]);
		features[s].push_back(dMax[s]);
		features[s].push_back(range[s]);
		features[s].push_back(dRange[s]);
		features[s].push_back(dAvg[s]);
		features[s].push_back(dAvgSlope[s]);
		features[s].push_back(dAvgNuclei[s]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(dSyl_Segment[s][seg]);
		for (int seg=0; seg<numOfSegment-1; seg++)
			features[s].push_back(dSeg_Segment[s][seg]);
	}
}

void EnergyFeXor::ResetFeature(int numOfSyllable)
{
	DUMP(__PRETTY_FUNCTION__);

	begin.clear();
	end.clear();
	slopeBegin.clear();
	slopeEnd.clear();
	min.clear();
	max.clear();
	avg.clear();
	avgSlope.clear();
	avgNuclei.clear();
	totalNuclei.clear();
	if (numOfSegment>0)
	{
		for (int s=0; s<numOfSyllable; s++)
			segment[s].clear();
		segment.clear();
	}
}

void EnergyFeXor::ResetDeltaFeature(int numOfSyllable)
{
	DUMP(__PRETTY_FUNCTION__);

	dBegin.clear();
	dEnd.clear();
	dbe.clear();
	ddbe.clear();
	dSlopeBegin.clear();
	dSlopeEnd.clear();
	dbeSlope.clear();
	ddbeSlope.clear();
	dMin.clear();
	dMax.clear();
	range.clear();
	dRange.clear();
	dAvg.clear();
	dAvgSlope.clear();
	dAvgNuclei.clear();
	if (numOfSegment>0)
	{
		for (int s=0; s<numOfSyllable; s++)
			dSyl_Segment[s].clear();
		dSyl_Segment.clear();
		for (int s=0; s<numOfSyllable-1; s++)
			dSeg_Segment[s].clear();
		dSeg_Segment.clear();
	}
}

/**
 *0.Initialize feature vectors as zero vectors\n
 *1.Extract intra-syllable features\n
 *2.Extract cross-syllable features\n
 *3.Save features
 */
void EnergyFeXor::Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta)
{
	DUMP(__PRETTY_FUNCTION__);

	InitializeFeature(u->GetNumberOfSyllables());
	GetFeature(u);
	SaveFeature(features, u->GetNumberOfSyllables());

	if (extractDelta)
	{
		InitializeDeltaFeature(u->GetNumberOfSyllables());
		GetDeltaFeature(u->GetNumberOfSyllables());
		SaveDeltaFeature(features, u->GetNumberOfSyllables());
		ResetDeltaFeature(u->GetNumberOfSyllables());
	}

	ResetFeature(u->GetNumberOfSyllables());
}

void EnergyFeXor::PrintDefinition(vector<string>& definition)
{
	definition.push_back("<Energy::begin> Energy of the first frame");
	definition.push_back("<Energy::end> Energy of the last frame");
	definition.push_back("<Energy::slopeBegin> Energy contour slope at the beginning of character");
	definition.push_back("<Energy::SlopeEnd> Energy contour slope at the end of character");
	definition.push_back("<Energy::min> Minimum energy");
	definition.push_back("<Energy::max> Maximum energy");
	definition.push_back("<Energy::avg> Average frame energy");
	definition.push_back("<Energy::avgSlope> Average the absolute value of slope curvature of energy contour");
	definition.push_back("<Energy::avgNuclei> Average nuclei frame energy");
	definition.push_back("<Energy::totalNuclei> Sum of nuclei frame energy");
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<Energy::segment["+int2str(s)+"]> Average energy of "+int2str(s)+"-th segment of nuclei");
}

void EnergyFeXor::PrintDeltaDefinition(vector<string>& definition)
{
	definition.push_back("<Energy::dBegin> Delta, energy of the first frame");
	definition.push_back("<Energy::dEnd> Delta, energy of the last frame");
	definition.push_back("<Energy::dbe> Difference between the energy values of the first and the last frame");
	definition.push_back("<Energy::ddbe> Delta, difference between the energy values of the first and last frame");

	definition.push_back("<Energy::dSlopeBegin> Delta, energy contour slope at the beginning");
	definition.push_back("<Energy::dSlopeEnd> Delta, energy contour slope at the end");
	definition.push_back("<Energy::dbeSlope> Difference between the energy contour slope of the beginning and the end");
	definition.push_back("<Energy::ddbeSlope> Delta, difference between the energy contour slope of the beginning and the end");

	definition.push_back("<Energy::dMin> Delta, minimum energy");
	definition.push_back("<Energy::dMax> Delta, maximum energy");
	definition.push_back("<Energy::range> Energy range (max-min)");
	definition.push_back("<Energy::dRange> Delta, energy range");

	definition.push_back("<Energy::dAvg> Delta, average frame energy");
	definition.push_back("<Energy::dAvgSlope> Delta, average the absolute value of slope curvature of energy contour");

	definition.push_back("<Energy::dAvgNuclei> Difference between the current and preceding average nuclei frame energy");
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<Energy::dSyl_segment["+int2str(s)+"]> Difference of energy of "+int2str(s)+"-th segment of nuclei of current syllable and previous syllable");
	for (int s=0; s<numOfSegment-1; s++)
		definition.push_back("<Energy::dSeg_segment["+int2str(s)+"]> Difference of energy of "+int2str(s)+"-th and "+int2str(s+1)+"-th segment of nuclei");
}
