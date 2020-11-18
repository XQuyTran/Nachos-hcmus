#include "syscall.h"
#include "copyright.h"

int main()
{
	// in tieu de
	PrintString("Code\tChar\n);
	
	// in cac ky tu in duoc, bao gom cac ky tu thuoc bang mo rong
	for (int i = 32; i < 256; i++)
	{
		PrintInt(i);
		PrintChar('\t');
		PrintChar((char)i));
		PrintChar('\n');
	}
	
	Halt();
}
