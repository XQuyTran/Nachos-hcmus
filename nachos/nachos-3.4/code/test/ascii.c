#include "syscall.h"
#include "copyright.h"

int main()
{
	int i;
	//char ch = (char)32;
	// in tieu de
	PrintString("ASCII Table\n");
	PrintString("Code\tChar\n");
	
	// in cac ky tu in duoc, bao gom cac ky tu thuoc bang mo rong
	for (i = 32; i < 127; i++)
	{
		PrintInt(i);
		PrintChar('\t');

		PrintChar((char)i);
		PrintChar('\n');
		//ch++;
	}
	
	Halt();
}
