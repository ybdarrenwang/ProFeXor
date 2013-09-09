#include "Config.h"

Config::Config(int argc, char** argv)
{
	InitConfig();

	if( argc < 2 )
	{
		Usage( argv[0] );
		exit(0);
	}

	vector<string> Args;
	for(int i = 1; i < argc; i ++ )
	{
		string tmpstr(argv[i]);
		Args.push_back( tmpstr );
	}
	ReadConfig(Args);
}

Config::~Config()
{
}

void Config::Usage( const char *prog )
{
	InitConfig();  // load defaults
	cout << "\n"
	     << "\"Pro\"sodic \"Fe\"ature e\"X\"tract\"or\"\n\n"
	     << "    by Yow-Bang Wang, NTU Speech Lab, 2013/07.\n\n"
	     << "Usage: " << prog << " [options] ...\n\n"
	     << "options:\n"
	     << "  -S PSDXEF                  select feature sets to be calculated\n"
	     << "                             P: log-f0\n"
	     << "                             S: SRI-stylized log-f0\n"
	     << "                             D: syllable duration\n"
	     << "                             X: pause duration, and the relations among pauses and syllables\n"
	     << "                             E: log-energy\n"
	     << "                             F: feature set used by Che-Kuang Lin for Disfluency detection\n"
	     << "                             (note: F overrides the other selections of feature sets!)\n"
	     << "\n"
	     << "  -D                         also extract delta-features (see README)\n"
	     << "\n"
	     << "  -numOfSegment n            also extract pitch/energy features which based on equi-partitioning\n"
	     << "                             nuclei into n segments. Set n=0 to disable.   (default: n=" << numOfSegment << ")\n"
	     << "\n"
	     << "  -m RECFile                 input REC file (see README)\n"
	     << "\n"
	     << "  -o featureFile             output feature file                           (default: \"" << outFeaFile << "\")\n"
	     << "\n"
	     << "  -d definitionFile          output feature definition file                (default: \"" << outDefFile << "\")\n"
	     << "\n"
	     << "  -noHeader                  set the wave files as no header               (default: " << (noHeader ? "true" : "false") << ")\n"
	     << "\n"
	     << "  -samplingRate r            set the sampling rate of wave files to r      (default: " << samplingRate << ")\n"
	     << "\n"
	     << "  -pitchNorm m [window_size] set pitch contour normalization mode to m     (default: " << normalizeMode << ")\n"
	     << "                             utt: subtract the mean pitch of the utterance\n"
	     << "                             win: subtract the mean pitch within the range of context window\n"
	     << "                             off: pitch contour won't be normalized\n"
	     << "\n"
	     << "  -energyNorm m              set energy contour normalization mode to m    (default: " << eNormalizeMode << ")\n"
	     << "                             max: subtract the max energy of the utterance\n"
	     << "                             off: energy contour won't be normalized\n"
	     << "\n"
	     << "  -pitchSmooth window_size   enable pitch contour moving window smoothing  (default: " << w_s << ")\n"
	     << "\n"
	     << "  -pitchInterpolate i        set pitch contour interpolation mode to i     (default: " << interpolateMode << ")\n"
	     << "                             spline: Cubic Spline interpolation\n"
	     << "                             linear: Linear interpolation\n"
	     << "\n"
	     << "  -t N                       set number of threads as N                    (default: " << numOfThread << ")\n"
	     << "\n"
	     << endl;
	exit(1);
}

void Config::InitConfig()
{
	outFeaFile = "feature.txt";
	outDefFile = "";
	pca_axe = "";
	noHeader = false;
	samplingRate = 16000;
	normalizeMode = "utt";
	eNormalizeMode = "max";
	interpolateMode = "spline";
	w_n = 50;
	w_s = 0;
	extractDelta = false;
	numOfSegment = 3;
	numOfThread = 1;
}

// Read command line option and set proper global var.
void Config::ReadConfig(vector<string> &Args)
{
	cout<<"< Read options >"<< endl;

	for( unsigned int i = 0; i < Args.size(); i ++ )
	{
		if( Args[i] == "-S" && Args.size() > i )
		{
			i++;
			featureSet = Args[i];
			cout<<"- Selected features: "<<Args[i]<<endl;
		}
		else if( Args[i] == "-D" && Args.size() > i )
		{
			extractDelta = true;
			cout<<"- Delta-features will also be extracted"<<endl;
		}
		else if( Args[i] == "-m" && Args.size() > i )
		{
			i++;
			inRECFile = Args[i];
			cout<<"- Time label file: "<<Args[i]<<endl;
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
		else if( Args[i] == "-energyNorm" && Args.size() > i )
		{
			i++;
			eNormalizeMode = Args[i];
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
		else if( Args[i] == "-o" && Args.size() > i )
		{
			i++;
			outFeaFile = Args[i];
		}
		else if( Args[i] == "-d" && Args.size() > i )
		{
			i++;
			outDefFile = Args[i];
			cout<<"- Output feature definition file: "<<outDefFile<<endl;
		}
		else if( Args[i] == "-noHeader" && Args.size() > i )
		{
			noHeader = true;
			cout<<"- Set input wave files as no header"<<endl;
		}
		else if( Args[i] == "-samplingRate" && Args.size() > i )
		{
			i++;
			stringstream ss(Args[i]);
			ss >> samplingRate;
			cout<<"- Set input wave files sampling rate="<<samplingRate<<endl;
		}
		else if( Args[i] == "-numOfSegment" && Args.size() > i )
		{
			i++;
			stringstream ss(Args[i]);
			ss >> numOfSegment;
		}
		else if( Args[i] == "-t" && Args.size() > i )
		{
			i++;
			stringstream ss(Args[i]);
			ss >> numOfThread;
			cout<<"- Set number of threads="<<numOfThread<<endl;
		}
	}

	cout<<"- Set number of equi-partitioning nuclei segment="<<numOfSegment<<endl;
	cout<<"- Output feature file: "<<outFeaFile<<endl;

	if (w_s > 0)	
		cout<<"- Pitch contour smoothing: moving window smoothing with window size = "<<w_s<<" frames"<<endl;
	else
		cout<<"- Pitch contour smoothing: disabled"<<endl;

	if (normalizeMode == "utt")
		cout<<"- Pitch contour normalization: subtracting mean pitch of each utterance"<<endl;
	else if (normalizeMode == "win")
		cout<<"- Pitch contour normalization: moving average subtraction with window size = "<<w_n<<" frames"<<endl;
	else if (normalizeMode == "off")
		cout<<"- Pitch contour normalization: disabled"<<endl;
	else
	{
		cerr<<"[Error] unknown pitch contour normalization mode: "<<normalizeMode<<endl;
		exit(1);
	}

	if (interpolateMode == "spline")
		cout<<"- Pitch contour interpolation: Cubic Spline"<<endl;
	else if (interpolateMode == "linear")
		cout<<"- Pitch contour interpolation: Linear"<<endl;
	else
	{
		cerr<<"[Error] unknown pitch contour interpolation mode: "<<interpolateMode<<endl;
		exit(1);
	}

	if (eNormalizeMode == "max")
		cout<<"- Energy contour normalization: subtracting max energy of each utterance"<<endl;
	else if (eNormalizeMode == "off")
		cout<<"- Energy contour normalization: disabled"<<endl;
	else
	{
		cerr<<"[Error] unknown energy contour normalization mode: "<<eNormalizeMode<<endl;
		exit(1);
	}
}
