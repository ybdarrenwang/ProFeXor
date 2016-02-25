#include "Config.h"
#include "Profexor.h"

using namespace std;

int main(int argc, char **argv)
{	
	Config cfg(argc, argv);
	ProFeXor* myProFeXor = new ProFeXor(&cfg);
	myProFeXor->ExtractFeature();
	myProFeXor->WriteDefinition();
	delete myProFeXor;
	return 0;
}
