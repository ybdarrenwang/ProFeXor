#ifndef SUBSYLLABLE_H
#define SUBSYLLABLE_H

/**
*This class is for the acoustic units smaller than a syllable\n
*, including onset, nuclei, coda, or any sub-segments of syllable.
*/
class SubSyllable : public AcousticUnit
{
	public:
		SubSyllable(int b, int e)
		{
			if (b>e)
			{
				cerr<<"[Error] (SubSyllable::SubSyllable) begin time > end time"<<endl;
				exit(1);
			}
			beginTime=b;
			endTime=e;
		}
};

#endif
