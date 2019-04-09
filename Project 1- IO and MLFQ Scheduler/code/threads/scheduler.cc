// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "debug.h"
#include "scheduler.h"
#include "main.h"

//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------

Scheduler::Scheduler()
{ 

    readyMultiList = new List<Thread *>[4];
    quantumV = kernel->getQuantum();
    toBeDestroyed = NULL;
    std::cout << "\n\n Using MLFQ scheduling with 4 queue levels-";
    std::cout << "\n Given Quantum values for each level of MLFQ :  ";
    for(int i=1; i  <= 4; i++){
        std::cout << (quantumV * i) << "  ";
    }
    
    Print();
} 

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
   /* for(int i=0; i<4 ; i++){
        delete readyMultiList.RemoveFront();
    } 
    */
   delete readyMultiList;
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread *thread)
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());

    if(thread->blocked){

        std::cout << "\n" << thread->getName() << " was running currently. Interrupted by higher priority I/O thread.";
        std::cout << "\n\n" << thread->getName() << " come to ready to run with quantum remain: " << thread->getQuant();
        readyMultiList[thread->getPriority()-1].Append(thread);
        thread->blocked = FALSE;
    }

    else{

        std::cout << "\n\n" << thread->getName() << " come to ready to run with quantum remain: " << thread->getQuant();

    int newPriority;

   if(thread->getPreempt() == true)
   {
       if(thread->getPriority() > 1)
       {
            newPriority = thread->getPriority() - 1;
            thread->setPriority(newPriority);
       }
   }
   else{
        if(thread->getPriority() < 4)
        {
            newPriority = thread->getPriority() + 1;
            thread->setPriority(newPriority);
        }
   }
    
  if(thread->getPriority() == 1){
    thread->setQuant(quantumV);
    readyMultiList[0].Append(thread);
  }
  else if(thread->getPriority() == 2){
    thread->setQuant(quantumV * 2);
    readyMultiList[1].Append(thread);  
  }
  else if(thread->getPriority() == 3){
    thread->setQuant(quantumV * 3);  
    readyMultiList[2].Append(thread);
  }
  else{
    thread->setQuant(quantumV * 4); 
    readyMultiList[3].Append(thread);
  }

}
    thread->setStatus(READY);
    std::cout << "\nIt will be added to queue level: " << thread->getPriority();

    Print();
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    Thread *nextRun;

    if(!readyMultiList[0].IsEmpty()){
        nextRun = readyMultiList[0].RemoveFront();
    }
    else if(!readyMultiList[1].IsEmpty()){
        nextRun = readyMultiList[1].RemoveFront();
    }
    else if(!readyMultiList[2].IsEmpty()){
        nextRun = readyMultiList[2].RemoveFront();
    }
    else if(!readyMultiList[3].IsEmpty()){
        nextRun = readyMultiList[3].RemoveFront();
    }
    else{
        nextRun = NULL;
    }

    return nextRun;
}

// This function periodically checks waiting time of all threads and if its above 1000 it is above 1000 ticks then it will move thread to upper level

void
Scheduler::HandleStarvation()
{
    for(int i=0; i < 4; i++){

        List<Thread *> *temp = new List<Thread *>();
        *temp = readyMultiList[i];
        ListIterator<Thread*>* iter = new ListIterator<Thread*>(temp);
        
        for (; !iter->IsDone(); iter->Next()) {

            if(kernel->currentThread != iter->Item()){

                iter->Item()->starvation += 100;

                if(iter->Item()->starvation == 3000 && i > 0){

                    readyMultiList[i-1].Append(iter->Item());
                    readyMultiList[i].Remove(iter->Item());
                    iter->Item()->setPriority((iter->Item()->getPriority() - 1));
                    iter->Item()->starvation = 0;

                    std::cout << "\n\n---Starvation: " << iter->Item()->getName() << " has not been on CPU for 3000 ticks so moving it to higher queue from level " << i+1 << " to level " << i;

                    Print();
                }
            }
        }
    }


}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

void
Scheduler::Run (Thread *nextThread, bool finishing)
{
    if(!(kernel->alarm))
    {
        kernel->alarm = new Alarm(FALSE);
    }

    Thread *oldThread = kernel->currentThread;
    
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {	// mark that we need to delete current thread
         ASSERT(toBeDestroyed == NULL);
	 toBeDestroyed = oldThread;
    }
    
    if (oldThread->space != NULL) {	// if this thread is a user program,
        oldThread->SaveUserState(); 	// save the user's CPU registers
	oldThread->space->SaveState();
    }
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow

    kernel->currentThread = nextThread;  // switch to the next thread
    nextThread->setStatus(RUNNING);      // nextThread is now running
    
    DEBUG(dbgThread, "Switching from: " << oldThread->getName() << " to: " << nextThread->getName());

   // std::cout << "\n\nStarvation of thread: " << nextThread->getName() << " is : " << nextThread->starvation;

    nextThread->starvation = 0;

    std::cout << "\n\n" << nextThread->getName() << " is removed from queue and selected to run next.";
    std::cout << "\nSwitching from: " << oldThread->getName() << " to: " << nextThread->getName();

    nextThread->selectTime = kernel->stats->totalTicks;
    
    // This is a machine-dependent assembly language routine defined 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".

    SWITCH(oldThread, nextThread);

    // we're back, running oldThread
      
    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();		// check if thread we were running
					// before this one has finished
					// and needs to be cleaned up
    
    if (oldThread->space != NULL) {	    // if there is an address space
        oldThread->RestoreUserState();     // to restore, do it.
	oldThread->space->RestoreState();
    }
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void
Scheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
	toBeDestroyed = NULL;
    }
}
 
//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print()
{
    cout << "\n\n  Multi level feedback queue contents:\n";
    for(int i=0; i < 4; i++){
        cout << "\n   Queue "<< i+1 << ": ";
            readyMultiList[i].Apply(ThreadPrint);
    }

    cout << "\n";

}
