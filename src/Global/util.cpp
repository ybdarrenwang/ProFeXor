#include "util.h"

using namespace std;

string int2str(int i)
{
	stringstream ss;
	string s;
	ss<<i;
	ss>>s;
	return s;
}

/**
* split string with spaces
*/
vector<string> split(const std::string& s)
{
	stringstream ss(s);
	vector<string> vec((istream_iterator<string>(ss)), istream_iterator<string>());
	return vec;
}


/**
* Directly calculate energy from wave file
*/
void LoadWave(string file_name, bool no_header, int sampling_rate, vector<double> &energy_data)
{
	int i,j, frame_num;
	double frame_len_sec = 0.025; // sec.
	double frame_shift_sec = 0.01; // sec.
	int frame_len, frame_shift; // # of sample
	int rate, size;
	WavFileForIO *myWav;
	char *path = new char[200];

	// read wave & Load wave info.
	stringstream sstream(file_name);
	sstream >> path;

	if (no_header)
	{
		myWav = new WavFileForIO();
		ifstream is(path);
		if(!is.is_open())
		{
			cerr<<"[Error] cannot open "<<file_name<<endl;
			exit(1);
		}
		
		// get length of file
		is.seekg (0, ios::end);
		size = myWav->myDataSize = is.tellg(); // # of sample
		size = (int)((double)size/2); // because the read() function has been "reinterpreted"
				                     // from "char" into "short", the original size is for "char"!!!
		is.seekg (0, ios::beg);
		myWav->mySampleRate = sampling_rate;

		// get data
		short* tmp_myData_short = new short[size];
		is.read(reinterpret_cast<char*>(tmp_myData_short), myWav->myDataSize);

		myWav->myData_short = tmp_myData_short;

		is.close();
	}
	else
	{
		myWav = new WavFileForIO(path);
		size = myWav->myDataSize; // # of sample
		size = (int)((double)size/2); // because the read() function has been "reinterpreted"
				                     // from "char" into "short", the original size is for "char"!!!
	}

	rate = myWav->mySampleRate;
	frame_len = (int)floor(frame_len_sec*rate);
	frame_shift = (int)floor(frame_shift_sec*rate);
	frame_num = (int)floor((double)(size-frame_len)/frame_shift) + 1;

	cout<<"< Read "<<file_name<<" with sampling rate = "<<rate<<" >"<<endl;
	
	// calculate frame energy
	vector<double> frame; // a queue storing sample energy values
	double energy = 0;
	short amplitude;

	i = 0; // the first frame
	for(j=0; j<frame_len; j++)
	{
		amplitude = myWav->myData_short[i*frame_shift+j];
		frame.push_back(amplitude*amplitude);
		energy += amplitude*amplitude;
	}
	if (energy != 0)
		energy_data.push_back(log10(energy));
	else
		energy_data.push_back(0);

	for(i=1; i<frame_num; i++) // the remaining frames
	{
		energy = 0;
		for(j=0; j<frame_shift; j++)
		{
			amplitude = myWav->myData_short[i*frame_shift+(frame_len-frame_shift)+j];
			frame.erase(frame.begin());
			frame.push_back(amplitude*amplitude);
		}
		
		for(j=0; j<frame_len; j++)
			energy += frame[j];

		if (energy != 0)
			energy_data.push_back(log10(energy));
		else
			energy_data.push_back(0);
	}

	frame.clear();
	delete [] path;
	delete myWav;
}

void LoadF0(string file_name, vector<double>& data)
{
	ifstream in_f0(file_name.c_str());
	if (!in_f0.is_open())
	{
		cerr<<"[Error] cannot open file "<<file_name<<endl;
		exit(1);
	}
	cout<<"< Read "<<file_name<<" >"<<endl;
	string line;
	while(getline(in_f0, line))
	{
		// Read formatted data from a string.
		float f0 = atof(split(line)[0].c_str());
		if(f0>1000 ||(f0>0 && f0<1e-5))
			cout<<"[Warning] abnormal f0 value: "<<line<<endl;
		data.push_back(f0);
	}
}
