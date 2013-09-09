#include "UtteranceBuilder.h"
#include "debug.h"

UtteranceBuilder::UtteranceBuilder(Config* c)
{
	DUMP("\n< Construct utterance builder >");

	utt = 0;
	noHeader = c->GetNoHeader();
	samplingRate = c->GetSamplingRate();

	if (c->GetSmoothWindowSize() > 0)
	{
		DUMP("- Pitch contour smoothing: moving window smoothing");
		pitchContourSmoother = new MovingWindowSmoother(c->GetSmoothWindowSize());
	}
	else
	{
		DUMP("- Pitch contour smoothing: disabled");
		pitchContourSmoother = 0;
	}

	if (c->GetNormalizeMode() == "utt")
	{
		DUMP("- Pitch contour normalization: subtracting mean pitch of each utterance");
		pitchContourNormalizer = new MeanSubtraction();
	}
	else if (c->GetNormalizeMode() == "win")
	{
		DUMP("- Pitch contour normalization: moving average subtraction");
		pitchContourNormalizer = new MovingWindowMeanSub(c->GetNormalizeWindowSize());
	}
	else if (c->GetNormalizeMode() == "off")
	{
		DUMP("- Pitch contour normalization: disabled");
		pitchContourNormalizer = 0;
	}
	else
	{
		cerr<<"[Error] unknown pitch contour normalization mode: "<<c->GetNormalizeMode()<<endl;
		exit(1);
	}

	if (c->GetInterpolateMode() == "spline")
	{
		DUMP("- Pitch contour interpolation: Cubic Spline");
		pitchContourInterpolater = new CubicSplineInterpolater();
	}
	else if (c->GetInterpolateMode() == "linear")
	{
		DUMP("- Pitch contour interpolation: Linear");
		pitchContourInterpolater = new LinearInterpolater();
	}
	else
	{
		cerr<<"[Error] unknown pitch contour interpolation mode: "<<c->GetInterpolateMode()<<endl;
		exit(1);
	}

	if (c->GetENormalizeMode() == "max")
	{
		DUMP("- Energy contour normalization: subtracting max energy of each utterance");
		energyContourNormalizer = new MaxSubtraction();
	}
	else if (c->GetENormalizeMode() == "off")
	{
		DUMP("- Energy contour normalization: disabled");
		energyContourNormalizer = 0;
	}
	else
	{
		cerr<<"[Error] unknown energy contour normalization mode: "<<c->GetENormalizeMode()<<endl;
		exit(1);
	}
}

UtteranceBuilder::~UtteranceBuilder()
{
	delete pitchContourSmoother;
	delete pitchContourInterpolater;
	delete pitchContourNormalizer;
	delete energyContourNormalizer;
	delete utt;
}

void UtteranceBuilder::NewUtterance()
{
	if (utt)
		delete utt;
	utt = new Utterance();
}

/**
 * Load energy contour from wav file, and perform normalization.
*/
void UtteranceBuilder::BuildEnergyContour(UtteranceInfo* info)
{
	energyContour.clear();
	LoadWave(info->GetWaveFile(), noHeader, samplingRate, energyContour);
	if (energyContourNormalizer != 0)
		energyContourNormalizer->Normalize(energyContour, info);
	utt->SetEnergyContour(energyContour);
}

/**
 * Load pitch contour from f0 file, take log, and perform normalization, interpolation and smoothing.
*/
void UtteranceBuilder::BuildPitchContour(UtteranceInfo* info)
{
	pitchContour.clear();
	LoadF0(info->GetF0File(), pitchContour);

	// check if the number of frames of pitch contour is the same as time label
	int numFrame = info->GetTimeLabel(info->GetNumUnit()-1, 3);
	if (pitchContour.size() != numFrame)
	{
		cerr<<"[Warning] ("+info->GetF0File()+") length of pitch contour is different from time label"<<endl;
		if (pitchContour.size() < numFrame)
			cerr<<"[Warning] ("+info->GetF0File()+") Append "<<numFrame-pitchContour.size()<<" frames at the end of pitch contour"<<endl;
		else
			cerr<<"[Warning] ("+info->GetF0File()+") Remove "<<pitchContour.size()-numFrame<<" frames from the back of pitch contour"<<endl;
		pitchContour.resize(numFrame);
	}

	Vector_log(pitchContour);
	if (pitchContourInterpolater != 0)
		pitchContourInterpolater->Interpolate(pitchContour);
	if (pitchContourSmoother != 0)
		pitchContourSmoother->Smooth(pitchContour);
	if (pitchContourNormalizer != 0)
		pitchContourNormalizer->Normalize(pitchContour, info);

	utt->SetPitchContour(pitchContour);
}

/**
 * Create units based on time labels
*/
void UtteranceBuilder::BuildAcousticUnit(UtteranceInfo* info)
{
	vector< vector<int> > rawTimeLabel = info->GetTimeLabel();
	Syllable* prevSyllable = 0;
	Pause* prevPause = 0;
	for (int row=0; row<rawTimeLabel.size(); row++)
	{
		if (rawTimeLabel[row][3] > pitchContour.size())
		{
			cerr<<"[Error] time label at row "<<row+1
			<<" ("<<rawTimeLabel[row][0]<<" "<<rawTimeLabel[row][1]<<" "
			<<rawTimeLabel[row][2]<<" "<<rawTimeLabel[row][3]
			<<") exceed pitch contour length ("<<pitchContour.size()<<")"<<endl;
			exit(1);
		}

		if (rawTimeLabel[row][3] > energyContour.size())
		{
			cerr<<"[Error] time label at row "<<row+1
			<<" ("<<rawTimeLabel[row][0]<<" "<<rawTimeLabel[row][1]<<" "
			<<rawTimeLabel[row][2]<<" "<<rawTimeLabel[row][3]
			<<") exceed energy contour length ("<<energyContour.size()<<")"<<endl;
			exit(1);
		}

		if (rawTimeLabel[row][1] == -1 & rawTimeLabel[row][2] == -1) // pause
		{
			if (rawTimeLabel[row][3]>rawTimeLabel[row][0])
				prevPause = new Pause(rawTimeLabel[row][0], rawTimeLabel[row][3]);
		}
		else // syllable
		{
			Syllable* syllable = new Syllable(rawTimeLabel[row][0], rawTimeLabel[row][1], rawTimeLabel[row][2], rawTimeLabel[row][3]);
			syllable->SetPrevSyllable(prevSyllable);
			syllable->SetPrevPause(prevPause);
			syllable->SetPitchContour(subVec(pitchContour, rawTimeLabel[row][0], rawTimeLabel[row][3]-1));
			syllable->SetEnergyContour(subVec(energyContour, rawTimeLabel[row][0], rawTimeLabel[row][3]-1));
			utt->AddSyllable(syllable);
			if (prevSyllable != 0)
				prevSyllable->SetNextSyllable(syllable);
			prevSyllable = syllable;
			prevPause = 0;
		}
	}
}
