// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
    name = debugName; // dat ten cho Lock de debug
    queue = new List; // khoi tao hang doi tien trinh
    stat = FREE; // khoi tao trang thai mo cho lock
    ownerThread = NULL; // khoi tao tien trinh quan ly la rong
}

// Phuong thuc huy doi tuong Lock
Lock::~Lock() {
    delete queue; // huy cap phat hang doi tien trinh
}

// Phuong thuc gan 1 thread lam thread quan ly Lock
void Lock::Acquire() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// ngat interrupts
    
    // Kiem tra thread hien tai dang quan ly lock
    // neu dung thi tra ve
    if (!isHeldByCurrentThread()) {
    	// Kiem tra Lock dang ranh
        if (stat == FREE) {
    		stat = BUSY; // thay doi trang thai sang ban
    		ownerThread = currentThread; // cap quyen quan ly Lock cho thread hien tai
    	}
    	else {
    		queue->Append((void*)currentThread);// dua thread hien tai vao hang doi
    		currentThread->Sleep(); // dua thread ve trang thai sleep
    	}
    }
    
    (void) interrupt->SetLevel(oldLevel); // mo lai cac interrupt
}

// Phuong thuc tra quyen quan ly Lock
void Lock::Release() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    // Kiem tra thread hien tai dang quan ly lock
    // neu khong thi ket thuc
    if (!isHeldByCurrentThread()) {
    	DEBUG('t', "The Current Thread does not own the lock !");
    }
    // kiem tra co thread trong hang doi
    else if (!queue->IsEmpty()) {
    	// lay ra 1 thread tu hang doi
    	// doi trang thai thread thanh san sang
    	Thread* thread;
    	
    	thread = (Thread*)queue->Remove();
    	scheduler->ReadyToRun(thread);
    	
    	ownerThread = thread; // chuyen quyen quan ly cho thread moi
    }
    else {
    	// doi trang thai Lock thanh ranh
    	// dat lai thread quan ly la rong
    	stat = FREE;
    	ownerThread = NULL;
    }
    
    (void) interrupt->SetLevel(oldLevel); // mo lai interrupt
}

// Phuong thuc kiem tra tien trinh co quan ly lock hay khong
bool Lock::isHeldByCurrentThread() {
    return ownerThread == currentThread;
}

Condition::Condition(char* debugName) { }
Condition::~Condition() { }
void Condition::Wait(Lock* conditionLock) { ASSERT(FALSE); }
void Condition::Signal(Lock* conditionLock) { }
void Condition::Broadcast(Lock* conditionLock) { }
