#include "main.h"
#include "IOHandler.h"


IOAlarm::IOAlarm(bool doRandom)
{
    timer = new Timer(doRandom, this);
}

void IOAlarm::CallBack(){

//std::cout << "\n IOAlarm call back received \n";
//ListIterator<Request*>* iter = new ListIterator<Request*>(kernel->eventQueue);

IOHandler iohandler;

while(!kernel->eventQueue->IsEmpty() && kernel->eventQueue->Front()->getCompletionTime() <= kernel->stats->totalTicks){

    Request* reqObj = kernel->eventQueue->RemoveFront();

        if(reqObj->getreadWrite() == ReadInt){
                iohandler.read(reqObj);
        }
        else{
                iohandler.write(reqObj);
        }
    }
}

