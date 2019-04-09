
#include "IOInterrupt.h"
#include "Request.h"
#include "main.h"
#include "interrupt.h"
#include <string>

// IO write interrupt function to create I/O write request
void IOInterrupt::IOWrite(char* s) {

	//std::cout << "\n inside IOWrite \n";
	int delay = kernel->stats->totalTicks + rand() % 50;
	Request* reqObj = new Request(kernel->currentThread, WriteInt, delay, s);

	std::cout << "\n\n--> I/O Write interrupt request from thread ";     kernel->currentThread->Print();
	std::cout << " is received with completion time: " << delay;

	kernel->eventQueue->Insert(reqObj);

	kernel->currentThread->setPreempt(true);
	std::cout << "\n" <<  kernel->currentThread->getName()  << " is blocked until I/O request is handled.";

	kernel->interrupt->SetLevel(IntOff);
	kernel->currentThread->Sleep(FALSE);
	kernel->interrupt->SetLevel(IntOn);
}


// IO read interrupt function to create I/O read request
char* IOInterrupt::IORead(){

	char *c;
	//std::cout << "\n inside IORead \n";
	int delay = kernel->stats->totalTicks + rand() % 100;
	Request* reqObj = new Request(kernel->currentThread, ReadInt, delay, c);

	std::cout << "\n\n--> I/O Read interrupt request from thread ";
	kernel->currentThread->Print();
	std::cout << " is received with completion time: " << delay;

	kernel->eventQueue->Insert(reqObj);

	kernel->currentThread->setPreempt(true);
	std::cout << "\n" <<  kernel->currentThread->getName()  << " is blocked until I/O request is handled.";

	kernel->interrupt->SetLevel(IntOff);
	kernel->currentThread->Sleep(FALSE);
	kernel->interrupt->SetLevel(IntOn);

	c = reqObj->getBuffer();

	return c;
}

