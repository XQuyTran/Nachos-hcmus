#include "../userprog/syscall.h"
#include "../threads/copyright.h"
#define maxlen 255

int main()
{
	char str[maxlen + 1];
	PrintString("Nhap vao 1 chuoi: ");
	ReadString(str, maxlen);
	PrintString("Chuoi vua nhap la: ");
	PrintString(str);
	PrintChar('\n');

	PrintString("Hello, World!\n");
	Halt();
}
