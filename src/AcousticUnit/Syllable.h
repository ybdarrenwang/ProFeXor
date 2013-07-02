#ifndef SYLLABLE_H
#define SYLLABLE_H

#include "AcousticUnit.h"
#include "Pause.h"
#include "SubSyllable.h"

class Syllable : public AcousticUnit
{
	public:
		Syllable(int a, int b, int c, int d)
		{
			if (b==c)
			{
				cerr<<"Error: nuclei duration should not be 0 ("<<a<<" "<<b<<" "<<c<<" "<<d<<")"<<endl;
				exit(1);
			}
			beginTime = a;
			endTime = d;
			onset = new SubSyllable(a,b);
			nuclei = new SubSyllable(b,c);
			coda = new SubSyllable(c,d);
			prevSyllable = 0;
			nextSyllable = 0;
			prevPause = 0;
		}

		~Syllable()
		{
			delete onset;
			delete nuclei;
			delete coda;
			delete prevPause;
		}

		void SetPitchContour(vector<double> c);
		void SetEnergyContour(vector<double> c);
		void SetPrevSyllable(Syllable* s){prevSyllable = s;}
		void SetNextSyllable(Syllable* s){nextSyllable = s;}
		void SetPrevPause(Pause* p){prevPause = p;}

		Syllable* GetPrevSyllable(){return prevSyllable;}
		Syllable* GetNextSyllable(){return nextSyllable;}
		Pause* GetPrevPause(){return prevPause;}
		SubSyllable* GetOnset(){return onset;}
		SubSyllable* GetNuclei(){return nuclei;}
		SubSyllable* GetCoda(){return coda;}

	private:
		Syllable* prevSyllable;
		Syllable* nextSyllable;
		Pause* prevPause;
		SubSyllable* onset;
		SubSyllable* nuclei;
		SubSyllable* coda;		
};

#endif
