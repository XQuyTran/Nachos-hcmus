// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

/*NoException,           // Everything ok!
		     SyscallException,      // A program executed a system call.
		     PageFaultException,    // No valid translation found
		     ReadOnlyException,     // Write attempted to page marked 
					    // "read-only"
		     BusErrorException,     // Translation resulted in an 
					    // invalid physical address
		     AddressErrorException, // Unaligned reference or one that was beyond the end of the address space
		     OverflowException,     // Integer overflow in add or sub.
		     IllegalInstrException, // Unimplemented or reserved instr.
		     
		     NumExceptionTypes*/

char* User2System(int virtAddr, int limit){
	int i;
	int oneChar;
	char* kernelBuf=NULL;
	kernelBuf=new char[limit+1];
	if(kernelBuf==NULL){
		return kernelBuf;	
	}
	memset(kernelBuf,0,limit+1);
	for(i=0;i<limit;i++){
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i]=(char)oneChar;
		if(oneChar==0)
			break;	
	}
	return kernelBuf;
}

int System2User(int virtAddr,int len, char* buffer)
{
	if(len<0)return -1;
	if(len==0) return len;
	int i=0;
	int oneChar=0;
	do{
		oneChar=(int)buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i++;	
	}while(i<len && oneChar!=0);
	return i;
}

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

    /*if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }*/
	switch (which){
	case NoException:
		return;
	case PageFaultException:{
		printf("No valid translation found\n");
		interrupt->Halt();
		break;
	}
	case ReadOnlyException:{
		printf("Write attempted to page marked \"read-only\"\n");
		interrupt->Halt();
		break;
	}
	case BusErrorException:{
		printf("Translation resulted in an invalid physical address\n");
		interrupt->Halt();
		break;
	}
	case AddressErrorException:{
		printf("Unaligned reference or one that was beyond the end of 			the address space\n"); 	
		interrupt->Halt();
		break;
	}
	case OverflowException:{
		printf("Integer overflow in add or sub\n"); 	
		interrupt->Halt();
		break;
	}
	case IllegalInstrException:{
		printf("Unimplemented or reserved instr\n"); 	
		interrupt->Halt();
		break;
	}
	case NumExceptionTypes:{
		printf("Error\n"); 	
		interrupt->Halt();
		break;
	}
	case SyscallException:{
		switch(type) {
		case SC_Halt: {
			DEBUG('a', "Shutdown, initiated by user program.\n");
   			interrupt->Halt();
			break;
		}
		case SC_Exit:
			break;
		case SC_Exec:
			break;
		case SC_Join:
			break;
		case SC_Create:{
			int virtAddr;
			char * filename;
			DEBUG('a',"\n SC_Create call...");
			DEBUG('a',"\n Reading virtual address of filename");
			virtAddr=machine->ReadRegister(4);
			DEBUG('a',"\nReading filename.");
			filename=User2System(virtAddr,33);	
			if(filename==NULL){
				printf("\n Not enough memory in system");
				DEBUG('a',"\nNot enough memory in system");
				machine->WriteRegister(2,-1);
				delete filename;
				return;
			}
			DEBUG('a',"\n Finish reading filename.");
			if(!fileSystem->Create(filename,0)){
				printf("\n Error create file '%s',filename");
				machine->WriteRegister(2,-1);
				delete filename;
				return;
			}
			machine->WriteRegister(2,0);
			delete filename;
			break;
		}
		case SC_Open:{
			int virtAddr =machine->ReadRegister(4);
			char* filename;
			filename=User2System(virtAddr,32);
			if(!fileSystem->Open(filename)){
				machine->WriteRegister(2,-1);
				delete filename;
				return;
			}
			machine->WriteRegister(2,0);
			delete filename;
			break;
		}
		case SC_Read:{
			
			break;
 		}
		case SC_Write:{
			
			break;
		}
		case SC_Close:
			break;
		case SC_Fork:
			break;
		case SC_Yield:
			break;
		default:{ 
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);	
		}
		}
	}
	}
}
