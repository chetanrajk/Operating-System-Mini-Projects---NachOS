#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "IOInterrupt.h"

void CpuIntensiveThread1() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : CPU Intensive thread started...";
   
    for(int i=0; i < 200; i++){
        kernel->interrupt->OneTick();
    }

    std::cout << "\n\n===> End of Thread-1 <===\n";

}

void IOOperation1() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : I/O read-write thread started...";

    IOInterrupt* obj = new IOInterrupt();

    obj->IOWrite(" CONTENTS TO WRITE ");

    char *readBuff;
    readBuff = obj->IORead();

    std::cout << "\n\n===> End of Thread-2 <===\n";
}

void readThread() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : I/O read only thread started...";

    IOInterrupt* obj = new IOInterrupt();

    char *readBuff;
    readBuff = obj->IORead();

    readBuff = obj->IORead();
    std::cout << "\n\n===> End of Thread-4 <===\n";
}

void CpuIntensiveThread2() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : CPU Intensive thread started...";
   
    for(int i=0; i < 200; i++){
        kernel->interrupt->OneTick();
    }

    std::cout << "\n\n===> End of Thread-3 <===\n";

}

void CpuIntensiveThread3() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : CPU Intensive thread started...";
   
    for(int i=0; i < 200; i++){
        kernel->interrupt->OneTick();
    }

    std::cout << "\n\n===> End of Thread-5 <===\n";

}

void CpuIntensiveThread4() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : CPU Intensive thread started...";
   
    for(int i=0; i < 200; i++){
        kernel->interrupt->OneTick();
    }

    std::cout << "\n\n===> End of Thread-8 <===\n";

}

void writeThread() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : I/O write only thread started...";

    IOInterrupt* obj = new IOInterrupt();

    obj->IOWrite(" CONTENTS TO WRITE ");

    obj->IOWrite(" CONTENTS TO WRITE ");

    std::cout << "\n\n===> End of Thread-6 <===\n";
}

void IOOperation2() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : I/O read-write thread started...";

    IOInterrupt* obj = new IOInterrupt();
    char *readBuff;
    readBuff = obj->IORead();

    obj->IOWrite(" CONTENTS TO WRITE ");

    std::cout << "\n\n===> End of Thread-9 <===\n";
}

void HybridThread1() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : Hybrid thread started (CPU and read)...";

    for(int i=0; i < 200; i++){
        kernel->interrupt->OneTick();
    }

    IOInterrupt* obj = new IOInterrupt();
    char *readBuff;
    readBuff = obj->IORead();
    std::cout << "\n\n===> End of Thread-7 <===\n";
}


void HybridThread2() {

    std::cout << "\n\n===> ";
    kernel->currentThread->Print();
    std::cout << " : Hybrid thread started (Write and CPU)...";

    IOInterrupt* obj = new IOInterrupt();

    obj->IOWrite(" CONTENTS TO WRITE ");

    for(int i=0; i < 200; i++){
        kernel->interrupt->OneTick();
    }

    std::cout << "\n\n===> End of Thread-10 <===\n";
}

void
ThreadTest()
{
    std::cout << "\n\n====================== MLFQ DEMO ===========================";

    Thread *t1 = new Thread("Thread-1");
    t1->Fork((VoidFunctionPtr)CpuIntensiveThread1, (void *) 1);

    Thread *t2 = new Thread("Thread-2");
    t2->Fork((VoidFunctionPtr)IOOperation1, (void *) 1);

    Thread *t3 = new Thread("Thread-3");
    t3->Fork((VoidFunctionPtr)CpuIntensiveThread2, (void *) 1);

    Thread *t4 = new Thread("Thread-4");
    t4->Fork((VoidFunctionPtr)readThread, (void *) 1);

    Thread *t5 = new Thread("Thread-5");
    t5->Fork((VoidFunctionPtr)CpuIntensiveThread3, (void *) 1);

    Thread *t6 = new Thread("Thread-6");
    t6->Fork((VoidFunctionPtr)writeThread, (void *) 1);

    Thread *t7 = new Thread("Thread-7");
    t7->Fork((VoidFunctionPtr)HybridThread1, (void *) 1);

    Thread *t8 = new Thread("Thread-8");
    t8->Fork((VoidFunctionPtr)CpuIntensiveThread4, (void *) 1);

    Thread *t9 = new Thread("Thread-9");
    t9->Fork((VoidFunctionPtr)IOOperation2, (void *) 1);

    Thread *t10 = new Thread("Thread-10");
    t10->Fork((VoidFunctionPtr)HybridThread2, (void *) 1);

}
