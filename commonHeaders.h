#include "IL/il.h"
#include "IL/ilu.h"
#include <iostream>
#include <windows.h>
#include <memory>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>


#include <boost\smart_ptr\shared_ptr.hpp>
#include <boost\smart_ptr\scoped_ptr.hpp>

using namespace std;

#ifndef SAFE_DELETE
	#define SAFE_DELETE(a) if(a != NULL) delete a; a = NULL;
#endif

#ifndef BIN_SIZE_ENUM
#define BIN_SIZE_ENUM
enum	BIN_SIZE {
		DEFAULT_BIN_SIZE	=	512,
		SMALL_BIN_SIZE		=	256,
		LARGE_BIN_SIZE		=	1024
};

#ifndef COMPARE_HEIGHTS
#define COMPARE_HEIGHTS
//This function is used as in the file map heights list sorting
bool compareSecond(pair<string, unsigned int> first, pair<string, unsigned int> second);
#endif

#endif