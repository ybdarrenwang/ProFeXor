#ifndef UTIL_H
#define UTIL_H

#include "global.h"
#include "wavIO.h"

using namespace std;

bool openFile( ifstream &, const char * );
bool openFile( ofstream &, const char * );
bool openFile( ifstream &f, string& FileName );
bool openFile( ofstream &f, string& FileName );

void removeLineFeed( string & );
void removeChar( string & , char );

vector<string> split(const std::string&);
vector<string> split(const std::string&, std::string);

bool getLine( istream& , string & );

string int2str(int i);

void LoadWave(string, bool, int, vector<double> &);
void LoadF0(string, vector<double> &);

#endif
