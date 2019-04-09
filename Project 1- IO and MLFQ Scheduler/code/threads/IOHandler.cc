
#include "IOHandler.h"
#include "main.h"

// Handler for I/O read operation

void IOHandler::read(Request* obj)
{
   // std::cout << "\n inside IO handler read";

    obj->setBuffer(" CONTENTS READ ");

    std::cout << "\n\n<-- I/O Read interrupt request from "; 
    obj->getCurrentThread()->Print();
    std::cout << " with completion time: "<< obj->getCompletionTime() << " is served at time: " << kernel->stats->totalTicks << "\nContents in buffer after..(Read operation) : " << obj->getBuffer() << "\n"; 
    std::cout << obj->getCurrentThread()->getName() << " is up now and will be added to queue.";

    kernel->scheduler->ReadyToRun(obj->getCurrentThread());

    int tempPriority = obj->getCurrentThread()->getPriority();
    tempPriority = (tempPriority == 1) ? 1 : (tempPriority - 1);

   if(tempPriority < kernel->currentThread->getPriority()){

     //  std::cout << "\n\n" << obj->getCurrentThread()->getName() << " will interrupt currently running thread as it is of low priority. ";

        kernel->currentThread->blocked = TRUE;
        
        Interrupt *interrupt = kernel->interrupt;
        MachineStatus status = interrupt->getStatus();
    
        if (status != IdleMode) {
	    interrupt->YieldOnReturn();
        } 

   }
 
}


// Handler for I/O write operation

void IOHandler::write(Request* obj)
{
   // std::cout << "\n inside IO handler write \n";
    char *s = obj->getBuffer();

    std::cout << "\n\n<-- I/O Write interrupt request from ";
    obj->getCurrentThread()->Print();
    std::cout << " with completion time: "<< obj->getCompletionTime() << " is served at: " << kernel->stats->totalTicks
     << "\nContents in buffer: " << s << "\n";
    std::cout << obj->getCurrentThread()->getName() << " is up now and will be added to queue.";

    kernel->scheduler->ReadyToRun(obj->getCurrentThread());

    int tempPriority = obj->getCurrentThread()->getPriority();
    tempPriority = (tempPriority == 1) ? 1 : (tempPriority - 1);

   if(tempPriority < kernel->currentThread->getPriority()){

    //   std::cout << "\n\n" << obj->getCurrentThread()->getName() << " will interrupt currently running thread as it is of low priority. ";

        kernel->currentThread->blocked = TRUE;
        
        Interrupt *interrupt = kernel->interrupt;
        MachineStatus status = interrupt->getStatus();
    
        if (status != IdleMode) {
	    interrupt->YieldOnReturn();
        } 

   }
}