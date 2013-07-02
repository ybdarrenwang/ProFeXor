#include <iostream> 
#include <fstream>
#include <sstream>
#include <map>
#include <set>    
#include <string>
#include <cstring>
#include <cstdlib>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>      
#include <stdio.h>   
#include <iomanip>   
#include <stdlib.h>  
#include <cmath>

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
