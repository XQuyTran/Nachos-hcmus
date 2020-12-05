#include "syscall.h"
#include "copyright.h"

int main()
{
	int i;
	char ch;
	// in tieu de
	PrintString("Code\tChar\n");
	
	// in cac ky tu in duoc, bao gom cac ky tu thuoc bang mo rong
	for (i = 32; i < 256; i++)
	{
		PrintInt(i);
		PrintChar('\t');
		ch = i;
		PrintChar(ch);
		PrintChar('\n');
	}
	
	Halt();
}
