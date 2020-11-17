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
#include <string.h>
void IncreasePC() 
{
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}



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

void ExceptionHandler(ExceptionType which)
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
		printf("Unaligned reference or one that was beyond the end of the address space\n"); 	
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
        
    		case SC_ReadInt: 
		{
			int MAX_BUFFER = 255;		// 1 line
			char* buffer = new char[MAX_BUFFER + 1];
			int numberofNum = gSynchConsole->Read(buffer, MAX_BUFFER);	// so chu so cua number
			int number = 0;		// ket qua


			bool isNegative = false;	// bien kiem tra number la so am hay duong
			int firstIndex = 0;		
			if (buffer[0] == '-') {
				isNegative = true;
				firstIndex = 1;

			} else if (buffer[0] == '+') {		// truong hop +223 = 223 van hop le
				firstIndex = 1;
			}

			// kiem tra so vua nhap co hop le hay khong
			for (int i = firstIndex; i < numberofNum; i++) {
				if (buffer[i] == '.') {			// 234.00000 van hop le
					for (int j = i + 1; j < numberofNum; j++) {
						if (buffer[j] != '0') {		// 123.01 khong hop le
							DEBUG('a', "\nThe integer number is invalid\n");
							printf("\n\n The integer numbber is invalid\n");
							machine->WriteRegister(2, 0);
							IncreasePC();
							delete buffer;
							return;	
						}     
					} 
					numberofNum = i;
					break;  
				} else if (buffer[i] < '0' || buffer[i] > '9') {	// number co ki tu khong phai la so
						DEBUG('a', "\nThe integer number is invalid\n");
						printf("\n\n The integer numbber is invalid\n");
						machine->WriteRegister(2, 0);
						IncreasePC();
						delete buffer;
						return;	
				} 	
			}


			// kiem tra so vua nhap co vuot nguong cho phep cua so nguyen
			if (firstIndex == 1) {		// truong hop co dau	
				if (numberofNum > 11) {		// Length(MAX_INT) = 10 them dau o buffer{0] nua se la 11
						DEBUG('a', "\nThe integer number is threshold\n");
						printf("\n\n The integer numbber is threshold INT_TYPE\n");
						machine->WriteRegister(2, 0);
						IncreasePC();
						delete buffer;
						return;	
				} else if (numberofNum == 11) {
					if (isNegative) {
						if (strcmp(buffer, "-2147483648") > 0) {
							DEBUG('a', "\nThe integer number is threshold\n");
							printf("\n\n The integer numbber is threshold MIN_INT\n");
							machine->WriteRegister(2, 0);
							IncreasePC();
							delete buffer;
							return;	
						} 
					} else {
						if (strcmp(buffer, "+2147483647") > 0) {
							DEBUG('a', "\nThe integer number is threshold\n");
							printf("\n\n The integer numbber is threshold MAX_INT\n");
							machine->WriteRegister(2, 0);
							IncreasePC();
							delete buffer;
							return;	
						}
					}	
				}
			} else {	// truong hop k dau
				if (numberofNum > 10) {		// Length(MAX_INT) = 10
					DEBUG('a', "\nThe integer number is threshold\n");
					printf("\n\n The integer numbber is threshold INT_TYPE\n");
					machine->WriteRegister(2, 0);
					IncreasePC();
					delete buffer;
					return;	
				} else if (numberofNum == 10) {
					if (strcmp(buffer, "2147483647") > 0) {
						DEBUG('a', "\nThe integer number is threshold\n");
						printf("\n\n The integer numbber is threshold MAX_INT\n");
						machine->WriteRegister(2, 0);
						IncreasePC();
						delete buffer;
						return;	
					}
				}
			}

			// chuyen chuoi ki tu ve dang so nguyen khi da hop le
			for (int i = firstIndex; i < numberofNum; i++) 
				number = number * 10 + (int)(buffer[i] - 48);	
			if (isNegative) {
				number *= -1;
			}
			machine->WriteRegister(2, number);
			IncreasePC();
			delete buffer;
			return;	
		}

		case SC_PrintInt: 
		{
			DEBUG('a', "SC_PrintInt called ...\n");
			int number = machine->ReadRegister(4);
			// Neu number = 0 thi print
			if (number == 0) {
				gSynchConsole->Write("0", 1);
				IncreasePC();
				return;
			}

			bool isNegative = false;	// bien kiem tra number la so am hay duong
			int numberofNum = 0;		// so chu so cua number
			int firstIndex = 0;		// vi tri bat dau luu tru chu so trong buffer

			// kiem tra number am hay duong
			if (number < 0) {
				number *= -1;
				isNegative = true;
				firstIndex = 1;		// index = 0 de luu ki hieu tru "-"
			}

			// dem so chu do cua number va luu vao bien numberofNum
			int nn = number;
			while (nn) {
				numberofNum++;
				nn /= 10;
			}

			// chuyen so nguyen ve dang ki tu luu vao vung nho buffer
			char* buffer = new char[numberofNum + firstIndex + 1];	
			for (int i = firstIndex + numberofNum - 1; i >= firstIndex; i--) {
				buffer[i] = (char)((number % 10) + 48);		// chuyen chu so ve ki tu 
				number /= 10;
			}

			if (isNegative) {
				buffer[0] = '-';
				buffer[numberofNum + 1] = 0;	// = NULL
			} else {
				buffer[numberofNum] = 0;	// = NULL
			}

			gSynchConsole->Write(buffer, numberofNum + firstIndex);
			IncreasePC();
			delete buffer;
			return;
		}
    
		default:{ 
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);	
		}
		}
	}
	}
}




