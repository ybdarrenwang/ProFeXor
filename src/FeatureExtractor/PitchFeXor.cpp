#include "PitchFeXor.h"

void PitchFeXor::InitializeFeature(int numOfSyllable)
{
	cout<<"Initializing features"<<endl;

	begin = vector<double>(numOfSyllable, 0);
	end = vector<double>(numOfSyllable, 0);
	slopeBegin = vector<double>(numOfSyllable, 0);
	slopeEnd = vector<double>(numOfSyllable, 0);
	min = vector<double>(numOfSyllable, 0);
	n_min = vector<double>(numOfSyllable, 0);
	max = vector<double>(numOfSyllable, 0);
	n_max = vector<double>(numOfSyllable, 0);
	avgPitch = vector<double>(numOfSyllable, 0);
	slopePitch = vector<double>(numOfSyllable, 0);
	avgSlope = vector<double>(numOfSyllable, 0);
	for (int s=0; s<numOfSyllable; s++)
	{
//		Legendre.push_back(vector<double>(3, 0));
		if (numOfSegment>0)
		{
			avgSegmentPitch.push_back(vector<double>(numOfSegment, 0));
			n_avgSegmentPitch.push_back(vector<double>(numOfSegment, 0));
			slopeSegmentPitch.push_back(vector<double>(numOfSegment, 0));
		}
	}
}

void PitchFeXor::InitializeDeltaFeature(int numOfSyllable)
{
	cout<<"Initializing delta-features"<<endl;

	d_begin = vector<double>(numOfSyllable, 0);
	d_end = vector<double>(numOfSyllable, 0);
	dbe = vector<double>(numOfSyllable, 0);
	d_dbe = vector<double>(numOfSyllable, 0);
	d_slopeBegin = vector<double>(numOfSyllable, 0);
	d_slopeEnd = vector<double>(numOfSyllable, 0);
	dbeSlope = vector<double>(numOfSyllable, 0);
	d_dbeSlope = vector<double>(numOfSyllable, 0);
	d_min = vector<double>(numOfSyllable, 0);
	d_max = vector<double>(numOfSyllable, 0);
	range = vector<double>(numOfSyllable, 0);
	d_range = vector<double>(numOfSyllable, 0);
	d_avgPitch = vector<double>(numOfSyllable, 0);
	d_slopePitch = vector<double>(numOfSyllable, 0);
	d_avgSlope = vector<double>(numOfSyllable, 0);
	if (numOfSegment>0)
	{
		for (int s=0; s<numOfSyllable; s++)
		{
			dSyl_avgSegmentPitch.push_back(vector<double>(numOfSegment, 0));
			dSeg_avgSegmentPitch.push_back(vector<double>(numOfSegment, 0));
			dSyl_slopeSegmentPitch.push_back(vector<double>(numOfSegment, 0));
			dSeg_slopeSegmentPitch.push_back(vector<double>(numOfSegment, 0));
		}
	}
}

void PitchFeXor::GetFeature(Utterance* u)
{
	cout<<"Get pitch features"<<endl;
	for (int syl=0; syl<u->GetNumberOfSyllables(); syl++)
	{
		SubSyllable* nuclei = u->GetSyllable(syl)->GetNuclei();
		int nucleiDuration = nuclei->GetDuration();
		if (nucleiDuration < 2)
		{
			cerr<<"Error: nuclei duration < 2, will be skipped"<<endl;
			continue;
		}

		vector<double> pitchContour = nuclei->GetPitchContour();

		// whole pitch contour
		avgPitch[syl] = getMean(pitchContour);
		slopePitch[syl] = getSlope(pitchContour);
		getMinMax(&(pitchContour[0]), min[syl], max[syl], nucleiDuration);
		n_min[syl] = min[syl] - avgPitch[syl];
		n_max[syl] = max[syl] - avgPitch[syl];
		avgSlope[syl] = 0;
		for(int i=1; i<pitchContour.size(); i++)
			avgSlope[syl] += abs(pitchContour[i]-pitchContour[i-1]);
		avgSlope[syl]/=(pitchContour.size()-1);

		// vowel boundary features
		begin[syl] = pitchContour[0];
		end[syl] = pitchContour[pitchContour.size()-1];
		slopeBegin[syl] = pitchContour[1]-pitchContour[0];
		slopeEnd[syl] = pitchContour[pitchContour.size()-1]-pitchContour[pitchContour.size()-2];

		// Legendre coefficient
//		Legendre_coeff(pitchContour, &(Legendre[syl][0]));

		// get equi-partitions of this syllable
		if (numOfSegment > 0)
		{
			if (nucleiDuration < numOfSegment*2)
			{
				cerr<<"Warning: duration of nuclei is too short to break into "<<numOfSegment<<" segments, will calculate the features of whole contour as of each contour-segment"<<endl;
				for (int seg=0; seg<numOfSegment; seg++)
				{
					avgSegmentPitch[syl][seg] = avgPitch[syl];
					n_avgSegmentPitch[syl][seg] = 0;
					slopeSegmentPitch[syl][seg] = slopePitch[syl];
				}
			}
			else
			{
				int segBeginTime = 0;
				int segEndTime = (int)floor((double)(nucleiDuration)/numOfSegment)-1;
				for (int seg=0; seg<numOfSegment; seg++)
				{
					vector<double> segmentPitchContour = subVec(pitchContour, segBeginTime, segEndTime);

					// segment pitch contour features
					avgSegmentPitch[syl][seg] = getMean(segmentPitchContour);
					n_avgSegmentPitch[syl][seg] = avgSegmentPitch[syl][seg] - avgPitch[syl];
					slopeSegmentPitch[syl][seg] = getSlope(segmentPitchContour);

					// update segment time boundary
					segBeginTime = segEndTime+1;
					segEndTime = (int)floor((seg+2)*((double)(nucleiDuration)/numOfSegment))-1;
				}
			}
		}
	}
}

void PitchFeXor::GetDeltaFeature(int numOfSyllable)
{
	cout<<"Get pitch delta-features"<<endl;

	for (int syl=0; syl<numOfSyllable; syl++)
	{
		range[syl] = max[syl]-min[syl];
		dbe[syl] = begin[syl]-end[syl];
		dbeSlope[syl] = slopeBegin[syl]-slopeEnd[syl];

		// cross-segment features
		if (numOfSegment>0)
		{
			if (syl>0) 
			{
				dSeg_avgSegmentPitch[syl][0] = avgSegmentPitch[syl][0] - avgSegmentPitch[syl-1][numOfSegment-1];
				dSeg_slopeSegmentPitch[syl][0] = slopeSegmentPitch[syl][0] - slopeSegmentPitch[syl-1][numOfSegment-1];
			}

			for (int seg=1; seg<numOfSegment; seg++)
			{
				dSeg_avgSegmentPitch[syl][seg] = avgSegmentPitch[syl][seg] - avgSegmentPitch[syl][seg-1];
				dSeg_slopeSegmentPitch[syl][seg] = slopeSegmentPitch[syl][seg] - slopeSegmentPitch[syl][seg-1];
			}
		}

		// cross-syllable features
		if (syl>0)
		{
			for (int seg=0; seg<numOfSegment; seg++)
			{
				dSyl_avgSegmentPitch[syl][seg] = avgSegmentPitch[syl][seg] - avgSegmentPitch[syl-1][seg];
				dSyl_slopeSegmentPitch[syl][seg] = slopeSegmentPitch[syl][seg] - slopeSegmentPitch[syl-1][seg];
			}

			d_begin[syl] = begin[syl] - begin[syl-1];
			d_end[syl] = end[syl] - end[syl-1];
			d_dbe[syl] = dbe[syl] - dbe[syl-1];

			d_slopeBegin[syl] = slopeBegin[syl] - slopeBegin[syl-1];
			d_slopeEnd[syl] = slopeEnd[syl] - slopeEnd[syl-1];
			d_dbeSlope[syl] = dbeSlope[syl] - dbeSlope[syl-1];

			d_min[syl] = min[syl] - min[syl-1];
			d_max[syl] = max[syl] - max[syl-1];
			d_range[syl] = range[syl] - range[syl-1];

			d_avgPitch[syl] = avgPitch[syl] - avgPitch[syl-1];
			d_slopePitch[syl] = slopePitch[syl] - slopePitch[syl-1];
			d_avgSlope[syl] = avgSlope[syl] - avgSlope[syl-1];
		}
	}
}

void PitchFeXor::SaveFeature(vector< vector<double> >& features, int numOfSyllable)
{
	cout<<"Saving pitch features"<<endl;

	for (int s=0; s<numOfSyllable; s++)
	{
		features[s].push_back(begin[s]);
		features[s].push_back(end[s]);
		features[s].push_back(slopeBegin[s]);
		features[s].push_back(slopeEnd[s]);
		features[s].push_back(min[s]);
		features[s].push_back(n_min[s]);
		features[s].push_back(max[s]);
		features[s].push_back(n_max[s]);
		features[s].push_back(avgPitch[s]);
		features[s].push_back(slopePitch[s]);
		features[s].push_back(avgSlope[s]);
//		for (int i=0; i<3; i++)
//			features[s].push_back(Legendre[s][i]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(avgSegmentPitch[s][seg]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(n_avgSegmentPitch[s][seg]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(slopeSegmentPitch[s][seg]);
	}
}

void PitchFeXor::SaveDeltaFeature(vector< vector<double> >& features, int numOfSyllable)
{
	cout<<"Saving pitch delta-features"<<endl;

	for (int s=0; s<numOfSyllable; s++)
	{
		features[s].push_back(d_begin[s]);
		features[s].push_back(d_end[s]);
		features[s].push_back(dbe[s]);
		features[s].push_back(d_dbe[s]);
		features[s].push_back(d_slopeBegin[s]);
		features[s].push_back(d_slopeEnd[s]);
		features[s].push_back(dbeSlope[s]);
		features[s].push_back(d_dbeSlope[s]);
		features[s].push_back(d_min[s]);
		features[s].push_back(d_max[s]);
		features[s].push_back(range[s]);
		features[s].push_back(d_range[s]);
		features[s].push_back(d_avgPitch[s]);
		features[s].push_back(d_slopePitch[s]);
		features[s].push_back(d_avgSlope[s]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(dSyl_avgSegmentPitch[s][seg]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(dSeg_avgSegmentPitch[s][seg]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(dSyl_slopeSegmentPitch[s][seg]);
		for (int seg=0; seg<numOfSegment; seg++)
			features[s].push_back(dSeg_slopeSegmentPitch[s][seg]);
	}
}

void PitchFeXor::ResetFeature(int numOfSyllable)
{
	cout<<"Reset PitchFeXor cache"<<endl;

	begin.clear();
	end.clear();
	slopeBegin.clear();
	slopeEnd.clear();
	min.clear();
	n_min.clear();
	max.clear();
	n_max.clear();
	avgPitch.clear();
	slopePitch.clear();
	avgSlope.clear();
	for (int s=0; s<numOfSyllable; s++)
	{
//		Legendre[s].clear();
		if (numOfSegment>0)
		{
			avgSegmentPitch[s].clear();
			n_avgSegmentPitch[s].clear();
			slopeSegmentPitch[s].clear();
		}
	}
//	Legendre.clear();
	if (numOfSegment>0)
	{
		avgSegmentPitch.clear();
		n_avgSegmentPitch.clear();
		slopeSegmentPitch.clear();
	}
}

void PitchFeXor::ResetDeltaFeature(int numOfSyllable)
{
	cout<<"Reset PitchFeXor cache"<<endl;

	d_begin.clear();
	d_end.clear();
	dbe.clear();
	d_dbe.clear();
	d_slopeBegin.clear();
	d_slopeEnd.clear();
	dbeSlope.clear();
	d_dbeSlope.clear();
	d_min.clear();
	d_max.clear();
	range.clear();
	d_range.clear();
	d_avgPitch.clear();
	d_slopePitch.clear();
	d_avgSlope.clear();
	if (numOfSegment>0)
	{
		for (int s=0; s<numOfSyllable; s++)
		{
			dSyl_avgSegmentPitch[s].clear();
			dSeg_avgSegmentPitch[s].clear();
			dSyl_slopeSegmentPitch[s].clear();
			dSeg_slopeSegmentPitch[s].clear();
		}
		dSyl_avgSegmentPitch.clear();
		dSeg_avgSegmentPitch.clear();
		dSyl_slopeSegmentPitch.clear();
		dSeg_slopeSegmentPitch.clear();
	}
}

/**
 *0.Initialize feature vectors as zero vectors\n
 *1.Extract intra-syllable features\n
 *2.Extract cross-syllable features\n
 *3.Save features
 */
void PitchFeXor::Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta)
{
	cout<<"=== Extracting pitch features ==="<<endl;
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

void PitchFeXor::PrintDefinition(vector<string>& definition)
{
	definition.push_back("<Pitch::begin> log-f0 of the first frame of vowel");
	definition.push_back("<Pitch::end> log-f0 of the last frame of vowel");
	definition.push_back("<Pitch::slopeBegin> Slope (difference) of log-f0 of the first two frames of vowel");
	definition.push_back("<Pitch::slopeEnd> Slope (difference) of log-f0 of the last two frames of vowel");
	definition.push_back("<Pitch::min> The smallest log-f0 value");
	definition.push_back("<Pitch::n_min> The smallest log-f0 value, normalized with average vowel log-f0");
	definition.push_back("<Pitch::max> The largest log-f0 value");
	definition.push_back("<Pitch::n_max> The largest log-f0 value, normalized with average vowel log-f0");
	definition.push_back("<Pitch::avgPitch> Average vowel log-f0");
	definition.push_back("<Pitch::slopeSyl> Vowel log-f0 contour slope");
	definition.push_back("<Pitch::avgSlope> Average the absolute value of slope (curvature) of log-f0 pitch contour");
/*	definition.push_back("<Pitch::Legendre[0]> The 1st Discrete Legendre coefficient of vowel pitch contour");
	definition.push_back("<Pitch::Legendre[1]> The 2nd Discrete Legendre coefficient of vowel pitch contour");
	definition.push_back("<Pitch::Legendre[2]> The 3rd Discrete Legendre coefficient of vowel pitch contour");*/
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<Pitch::avgSegmentPitch["+int2str(s)+"]> Average log-f0 of the "+int2str(s+1)+"-th segment of "+int2str(numOfSegment)+"-segmented pitch contour");
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<Pitch::n_avgSegmentPitch["+int2str(s)+"]> Average log-f0 of the "+int2str(s+1)+"-th segment of "+int2str(numOfSegment)+"-segmented pitch contour, subtracted by syllable mean pitch");
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<Pitch::slopeSegmentPitch["+int2str(s)+"]> Slope of log-f0 of the "+int2str(s+1)+"-th segment of "+int2str(numOfSegment)+"-segmented pitch contour");
}

void PitchFeXor::PrintDeltaDefinition(vector<string>& definition)
{
	definition.push_back("<Pitch::d_begin> Delta, log-f0 of the first frame of vowel");
	definition.push_back("<Pitch::d_end> Delta, log-f0 of the last frame of vowel");
	definition.push_back("<Pitch::dbe> Differnece of log-f0 between the first and last frame of vowel");
	definition.push_back("<Pitch::d_dbe> Delta, difference of log-f0 between the first and last frame of vowel");
	definition.push_back("<Pitch::d_slopeBegin> Delta, slope (difference) of log-f0 of the first two frames of vowel");
	definition.push_back("<Pitch::d_slopeEnd> Delta, slope (difference) of log-f0 of the first two frames of vowel");
	definition.push_back("<Pitch::dbeSlope> Difference between the slopes of log-f0 of the first two and last two frames of vowel");
	definition.push_back("<Pitch::d_dbeSlope> Delta, difference between the slopes of log-f0 of the first two and last two frames of vowel");
	definition.push_back("<Pitch::d_min> Delta, average of the smallest log-f0 values");
	definition.push_back("<Pitch::d_max> Delta, average of the largest log-f0 values");
	definition.push_back("<Pitch::range> Difference between the smallest and largest log-f0 values");
	definition.push_back("<Pitch::d_range> Delta, difference between the smallest and largest log-f0 values");
	definition.push_back("<Pitch::d_avgPitch> Delta, average vowel log-f0");
	definition.push_back("<Pitch::d_slopePitch> Delta, vowel log-f0 contour slope");
	definition.push_back("<Pitch::d_avgSlope> Average the absolute value of slope (curvature) of log-f0 pitch contour");

	if (numOfSegment > 0)
	{
		for (int s=0; s<numOfSegment; s++)
			definition.push_back("<Pitch::dSyl_avgSegmentPitch["+int2str(s)+"]> Differnce between the average log-f0 of the "+int2str(s+1)+"-th segment of current and previous syllable");
		definition.push_back("<Pitch::dSeg_avgSegmentPitch[0]> Differnce between the average log-f0 of the 1-st segment of current syllable and the last segment of previous syllable");
		for (int s=1; s<numOfSegment; s++)
			definition.push_back("<Pitch::dSeg_avgSegmentPitch["+int2str(s)+"]> Differnce between the average log-f0 of the "+int2str(s)+"-th segment and "+int2str(s+1)+"-th segment");
		for (int s=0; s<numOfSegment; s++)
			definition.push_back("<Pitch::dSyl_slopeSegmentPitch["+int2str(s)+"]> Differnce between the slope of log-f0 of the "+int2str(s+1)+"-th segment of current and previous syllable");
		definition.push_back("<Pitch::dSeg_slopeSegmentPitch[0]> Differnce between the slope of log-f0 of the 1-st segment of current syllable and the last segment of previous syllable");
		for (int s=1; s<numOfSegment; s++)
			definition.push_back("<Pitch::dSeg_slopeSegmentPitch["+int2str(s)+"]> Differnce between the slope of log-f0 of the "+int2str(s)+"-th segment and "+int2str(s+1)+"-th segment");
	}
}
