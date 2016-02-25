#ifndef UTIL_H
#define UTIL_H

#include "global.h"
#include "wavIO.h"

using namespace std;

string int2str(int i);
vector<string> split(const std::string&);
void LoadWave(string, bool, int, vector<double> &);
void LoadF0(string, vector<double> &);

#endif
