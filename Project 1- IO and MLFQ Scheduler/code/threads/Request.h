#include "thread.h"
#include "interrupt.h"

// This class provides request structure

class Request
{

public:

	Request(Thread* t, IntType rw, int delay, char *c);

	Thread* getCurrentThread() {
		return callingThread;
	}

	IntType getreadWrite() {
		return readWrite;
	}

	int getCompletionTime() {
		return completionTime;
	}

	char* getBuffer() {
		return buffer;
	}

	void setBuffer(char* c){
		buffer = c;
	}

private:

	Thread * callingThread;
	IntType readWrite;
	int completionTime;
	char *buffer;
};