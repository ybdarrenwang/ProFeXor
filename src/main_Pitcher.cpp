#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>   
#include <iomanip>   
#include <stdlib.h>  
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Normalizer.h"
#include "MeanSubtraction.h"
#include "MovingWindowMeanSub.h"
#include "MaxSubtraction.h"
#include "ContourSmoother.h"
#include "MovingWindowSmoother.h"
#include "ContourInterpolater.h"
#include "CubicSplineInterpolater.h"
#include "LinearInterpolater.h"
#include "Config.h"
#include "util.h"
#include "mymath.h"

using namespace std;

void usage(const char *prog, const string &normalizeMode, const int &w_s, const string &interpolateMode)
{
	cout<< "\n"
		<< "\"Pitcher\" for pitch contour pre-processing, by Darren Yow-Bang Wang, 2013/07\n\n"
		<< "Perform pitch contour interpolation, smoothing and normalization.\n\n"
		<< "Usage: " << prog << " [options] ...\n"
		<< "Options:\n"
		<< "  -i file_list               each row are one input pitch file and its corresponding output\n"
		<< "                             pitch file, separated by whitespace\n"
	    << "  -pitchNorm m [window_size] set pitch contour normalization mode to m     (default: " << normalizeMode << ")\n"
	    << "                             utt: subtract the mean pitch of the utterance\n"
	    << "                             win: subtract the mean pitch within the range of context window\n"
	    << "                             off: pitch contour won't be normalized\n"
	    << "\n"
	    << "  -pitchSmooth window_size   enable pitch contour moving window smoothing  (default: " << w_s << ")\n"
	    << "\n"
	    << "  -pitchInterpolate i        set pitch contour interpolation mode to i     (default: " << interpolateMode << ")\n"
	    << "                             spline: Cubic Spline interpolation\n"
	    << "                             linear: Linear interpolation\n"
	    << "\n"
	    << endl;
	exit(1);
}

void ReadArguments(int argc, char **argv, string &in_list, string &interpolateMode, string &normalizeMode, int &w_n, int &w_s)
{
	// read arguments
	if( argc < 2 )
	{
		usage(argv[0], normalizeMode, w_s, interpolateMode);
		exit(0);
	}

	vector<string> Args;
	for( int i = 1; i < argc; i ++ )
	{
		string tmpstr(argv[i]);
		Args.push_back( tmpstr );
	}

	cout << "====== Read options ======" << endl;
	for( unsigned int i = 0; i < Args.size(); i++ )
	{
		if( Args[i] == "-i" && Args.size() > i )
		{
			i++;
			in_list = Args[i];
			cout<<"Input file list: "<<in_list<<endl;
		}
		else if( Args[i] == "-pitchNorm" && Args.size() > i )
		{
			i++;
			normalizeMode = Args[i];
			if (normalizeMode == "win")
			{
				i++;
				stringstream ss(Args[i]);
				ss >> w_n;
			}
		}
		else if( Args[i] == "-pitchInterpolate" && Args.size() > i )
		{
			i++;
			interpolateMode = Args[i];
		}
		else if( Args[i] == "-pitchSmooth" && Args.size() > i )
		{
			i++;
			stringstream ss(Args[i]);
			ss >> w_s;
		}
	}
}

/**
 * Load pitch contour from f0 file, take log, and perform normalization, interpolation and smoothing.\n
 * Note: Because Normalizer::Normalize() requires UtteranceInfo* for the begin/end time of utterance,
 * here I create a "fake" UtteranceInfo* info which only records the beginning and end silence of 
 * utterance.
*/
int main(int argc, char **argv)
{	
	string in_list = "";
	ContourSmoother* pitchContourSmoother=0;
	ContourInterpolater* pitchContourInterpolater=0;
	Normalizer* pitchContourNormalizer=0;

	// set default arguments
	string normalizeMode = "utt", interpolateMode = "spline";
	int w_n = 50, w_s = 0;

	// Load arguments
	ReadArguments(argc, argv, in_list, interpolateMode, normalizeMode, w_n, w_s);

	// initialize pitch processors
	if (w_s > 0)	
	{
		cout<<"Pitch contour smoothing: moving window smoothing with window size = "<<w_s<<" frames"<<endl;
		pitchContourSmoother = new MovingWindowSmoother(w_s);
	}
	else
	{
		cout<<"Pitch contour smoothing: disabled"<<endl;
		pitchContourSmoother = 0;
	}

	if (normalizeMode == "utt")
	{
		cout<<"Pitch contour normalization: subtracting mean pitch of each utterance"<<endl;
		pitchContourNormalizer = new MeanSubtraction();
	}
	else if (normalizeMode == "win")
	{
		cout<<"Pitch contour normalization: moving average subtraction with window size = "<<w_n<<" frames"<<endl;
		pitchContourNormalizer = new MovingWindowMeanSub(w_n);
	}
	else if (normalizeMode == "off")
	{
		cout<<"Pitch contour normalization: disabled"<<endl;
		pitchContourNormalizer = 0;
	}
	else
	{
		cerr<<"[Error] unknown pitch contour normalization mode: "<<normalizeMode<<endl;
		exit(1);
	}

	if (interpolateMode == "spline")
	{
		cout<<"Pitch contour interpolation: Cubic Spline"<<endl;
		pitchContourInterpolater = new CubicSplineInterpolater();	
	}
	else if (interpolateMode == "linear")
	{
		cout<<"Pitch contour interpolation: Linear"<<endl;
		pitchContourInterpolater = new LinearInterpolater();
	}
	else
	{
		cerr<<"[Error] unknown pitch contour interpolation mode: "<<interpolateMode<<endl;
		exit(1);
	}

	// Prepare for I/O
	ifstream ifs_list;
	if (!openFile(ifs_list, in_list))
	{
		cerr<<"[Error] cannot open file list "<<in_list<<endl;
		exit(1);
	}

	// Begin processing
	vector<double> pitchContour;
	UtteranceInfo info;
	string line;
	vector<string> ioFiles;
	while(!getLine(ifs_list, line))
	{
		ioFiles = split(line);
		LoadF0(ioFiles[0], pitchContour);

		// create fake utterance information
		vector<int> fakeTimeLabel(4, 0);
		int uttBeginTime=0, uttEndTime=pitchContour.size()-1;
		while (uttBeginTime<pitchContour.size() & pitchContour[uttBeginTime]==0)
			++uttBeginTime;
		while (uttEndTime>0 & pitchContour[uttEndTime]==0)
			--uttEndTime;
		fakeTimeLabel[3] = uttBeginTime;
		info.AddUnit(fakeTimeLabel);
		fakeTimeLabel[0] = uttEndTime+1;
		info.AddUnit(fakeTimeLabel);

		// take log, perform normalization, interpolation and smoothing
		Vector_log(pitchContour);
		if (pitchContourInterpolater != 0)
			pitchContourInterpolater->Interpolate(pitchContour);
		if (pitchContourSmoother != 0)
			pitchContourSmoother->Smooth(pitchContour);
		if (pitchContourNormalizer != 0)
			pitchContourNormalizer->Normalize(pitchContour, &info);

		// output
		ofstream ofs_f0;
		if (!openFile(ofs_f0, ioFiles[1]))
		{
			cerr<<"[Error] cannot open "<<ioFiles[1]<<endl;
			return 1;
		}

		cout<<"Writing "<<ioFiles[1]<<"......"<<endl;
		for (int i=0; i<pitchContour.size(); i++)
			ofs_f0<<pitchContour[i]<<"\n";
		ofs_f0.close();
		
		pitchContour.clear();
		info.Clear();
	}

	return 0;
}
