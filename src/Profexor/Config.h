#ifndef CONFIG_H
#define CONFIG_H

#include "global.h"

using namespace std;

class Config
{
	public:
		Config(int, char**);
		~Config();

		string GetFeatureSet(){return featureSet;}
		string GetRECFile(){return inRECFile;}
		string GetOutputFeatureFile(){return outFeaFile;}
		string GetOutputDefinitionFile(){return outDefFile;}
		bool GetNoHeader(){return noHeader;}
		bool GetSamplingRate(){return samplingRate;}
		int GetSmoothWindowSize(){return w_s;}
		int GetNormalizeWindowSize(){return w_n;}
		string GetNormalizeMode(){return normalizeMode;}
		string GetENormalizeMode(){return eNormalizeMode;}
		string GetInterpolateMode(){return interpolateMode;}
		bool GetExtractDelta(){return extractDelta;}
		int GetNumOfSegment(){return numOfSegment;}

	protected:
		void Usage(const char *);
		void InitConfig();
		void ReadConfig(vector<string> &);

	private:
		string featureSet;
		string inRECFile;
		string outFeaFile;
		string outDefFile;
		string pca_axe;

		bool noHeader;
		int samplingRate;
		string normalizeMode;
		string eNormalizeMode;
		string interpolateMode;
		int w_n; ///<Window size for normalization
		int w_s; ///<Window size for smoothing
		bool extractDelta;
		int numOfSegment;
};

#endif
