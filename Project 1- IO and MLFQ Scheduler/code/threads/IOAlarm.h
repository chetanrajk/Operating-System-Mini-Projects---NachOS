
#include "callback.h"
#include "timer.h"

// This class provides callback for I/O events

class IOAlarm : public CallBackObj {
	
public:

	IOAlarm(bool doRandomYield);
	void CallBack();

private:

	Timer *timer;	
};