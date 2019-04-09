
#include "Request.h"


Request::Request(Thread* t, IntType rw, int delay, char *c) {

	//std::cout << "\n obj created \n";
	callingThread = t;
	readWrite = rw;
	completionTime = delay;
	buffer = c;
}