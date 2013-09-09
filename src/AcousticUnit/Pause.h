#ifndef	PAUSE_H
#define PAUSE_H

#include "AcousticUnit.h"

class Pause : public AcousticUnit
{
	public:
		Pause(int b, int e)
		{
			if (b>e)
			{
				cerr<<"[Error] (Pause::Pause) begin time > end time"<<endl;
				exit(1);
			}
			beginTime = b;
			endTime = e;
		}
};

#endif
