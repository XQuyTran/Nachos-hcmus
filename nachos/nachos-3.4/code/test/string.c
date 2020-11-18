#include "../userprog/syscall.h"
#include "../threads/copyright.h"
#define maxlen 255

int main()
{
	char str[maxlen + 1];

	ReadString(str, maxlen);
	PrintString(str);

	PrintString("Hello, World!");
	Halt();
}
