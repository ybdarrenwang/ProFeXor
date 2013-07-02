#include "SRIPitchFeXor.h"

/**
 *0.Initialize feature vectors as zero vectors\n
 *1.Extract intra-syllable features\n
 *2.Extract cross-syllable features\n
 *3.Save features
 */
void SRIPitchFeXor::Extract(Utterance* u, vector< vector<double> >& features, bool extractDelta)
{
	cout<<"=== Extracting SRI-stylized pitch features ==="<<endl;
	vector<double> backupPitchContour = u->GetPitchContour();
	stylizer->Stylize(u);
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
	u->SetPitchContour(backupPitchContour);
}

void SRIPitchFeXor::PrintDefinition(vector<string>& definition)
{
	definition.push_back("<SRIPitch::begin> log-f0 of the first frame of vowel, normalized with average sentence log-f0");
	definition.push_back("<SRIPitch::end> log-f0 of the last frame of vowel, normalized with average sentence log-f0");
	definition.push_back("<SRIPitch::slopeBegin> Slope (difference) of log-f0 of the first two frames of vowel");
	definition.push_back("<SRIPitch::slopeEnd> Slope (difference) of log-f0 of the last two frames of vowel");
	definition.push_back("<SRIPitch::min> The smallest log-f0 value, normalized with average sentence log-f0");
	definition.push_back("<SRIPitch::n_min> The smallest log-f0 value, normalized with average vowel log-f0");
	definition.push_back("<SRIPitch::max> The smallest log-f0 value, normalized with average sentence log-f0");
	definition.push_back("<SRIPitch::n_max> The largest log-f0 value, normalized with average vowel log-f0");
	definition.push_back("<SRIPitch::avgPitch> Average vowel log-f0, normalized with average sentence log-f0");
	definition.push_back("<SRIPitch::slopeSyl> Vowel log-f0 contour slope");
	definition.push_back("<SRIPitch::avgSlope> Average the absolute value of slope (curvature) of log-f0 pitch contour");
/*	definition.push_back("<SRIPitch::Legendre[0]> The 1st Discrete Legendre coefficient of vowel pitch contour");
	definition.push_back("<SRIPitch::Legendre[1]> The 2nd Discrete Legendre coefficient of vowel pitch contour");
	definition.push_back("<SRIPitch::Legendre[2]> The 3rd Discrete Legendre coefficient of vowel pitch contour");*/
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<SRIPitch::avgSegmentPitch["+int2str(s)+"]> Average log-f0 of the "+int2str(s+1)+"-th segment of "+int2str(numOfSegment)+"-segmented pitch contour");
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<SRIPitch::n_avgSegmentPitch["+int2str(s)+"]> Average log-f0 of the "+int2str(s+1)+"-th segment of "+int2str(numOfSegment)+"-segmented pitch contour, subtracted by syllable mean pitch");
	for (int s=0; s<numOfSegment; s++)
		definition.push_back("<SRIPitch::slopeSegmentPitch["+int2str(s)+"]> Slope of log-f0 of the "+int2str(s+1)+"-th segment of "+int2str(numOfSegment)+"-segmented pitch contour");
}

void SRIPitchFeXor::PrintDeltaDefinition(vector<string>& definition)
{
	definition.push_back("<SRIPitch::d_begin> Delta, log-f0 of the first frame of vowel");
	definition.push_back("<SRIPitch::d_end> Delta, log-f0 of the last frame of vowel");
	definition.push_back("<SRIPitch::dbe> Differnece of log-f0 between the first and last frame of vowel");
	definition.push_back("<SRIPitch::d_dbe> Delta, difference of log-f0 between the first and last frame of vowel");
	definition.push_back("<SRIPitch::d_slopeBegin> Delta, slope (difference) of log-f0 of the first two frames of vowel");
	definition.push_back("<SRIPitch::d_slopeEnd> Delta, slope (difference) of log-f0 of the first two frames of vowel");
	definition.push_back("<SRIPitch::dbeSlope> Difference between the slopes of log-f0 of the first two and last two frames of vowel");
	definition.push_back("<SRIPitch::d_dbeSlope> Delta, difference between the slopes of log-f0 of the first two and last two frames of vowel");
	definition.push_back("<SRIPitch::d_min> Delta, average of the smallest log-f0 values");
	definition.push_back("<SRIPitch::d_max> Delta, average of the largest log-f0 values");
	definition.push_back("<SRIPitch::range> Difference between the smallest and largest log-f0 values");
	definition.push_back("<SRIPitch::d_range> Delta, difference between the smallest and largest log-f0 values");
	definition.push_back("<SRIPitch::d_avgPitch> Delta, average vowel log-f0");
	definition.push_back("<SRIPitch::d_slopePitch> Delta, vowel log-f0 contour slope");
	definition.push_back("<SRIPitch::d_avgSlope> Average the absolute value of slope (curvature) of log-f0 pitch contour");

	if (numOfSegment > 0)
	{
		for (int s=0; s<numOfSegment; s++)
			definition.push_back("<SRIPitch::dSyl_avgSegmentPitch["+int2str(s)+"]> Differnce between the average log-f0 of the "+int2str(s+1)+"-th segment of current and previous syllable");
		definition.push_back("<SRIPitch::dSeg_avgSegmentPitch[0]> Differnce between the average log-f0 of the 1-st segment of current syllable and the last segment of previous syllable");
		for (int s=1; s<numOfSegment; s++)
			definition.push_back("<SRIPitch::dSeg_avgSegmentPitch["+int2str(s)+"]> Differnce between the average log-f0 of the "+int2str(s)+"-th segment and "+int2str(s+1)+"-th segment");
		for (int s=0; s<numOfSegment; s++)
			definition.push_back("<SRIPitch::dSyl_slopeSegmentPitch["+int2str(s)+"]> Differnce between the slope of log-f0 of the "+int2str(s+1)+"-th segment of current and previous syllable");
		definition.push_back("<SRIPitch::dSeg_slopeSegmentPitch[0]> Differnce between the slope of log-f0 of the 1-st segment of current syllable and the last segment of previous syllable");
		for (int s=1; s<numOfSegment; s++)
			definition.push_back("<SRIPitch::dSeg_slopeSegmentPitch["+int2str(s)+"]> Differnce between the slope of log-f0 of the "+int2str(s)+"-th segment and "+int2str(s+1)+"-th segment");
	}
}
