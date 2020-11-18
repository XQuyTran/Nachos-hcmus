#include "syscall.h"
#include "copyright.h"
#define nameLen 31

int main()
{
	char name1[namelen] = "Name 1";
	char name2[namelen] = "Name 2";
	char name3[namelen] = "Name 3";
	char name4[namelen] = "Name 4";

	char* names[4] = {name1, name2, name3, name4};
	int id[4] = {1,2,3,4};

	PrintString("Thong tin thanh vien trong nhom\n");
	PrintString("Ho ten\tMSSV\n");

	for (int i = 0; i < 4; i++)
	{
		PrintString(names[i]);
		PrintString("\t");
		PrintInt(id[i]);
	}

	PrintString("Chuong trinh ascii: in ra bang ma ASCII cua cac ky tu doc duoc.\n");
	PrintString("Chuong trinh sort: sap xep mang n so nguyen do nguoi dung nhap vao theo chieu tang hoac giam dan (tuy chon).\n");
	Halt();
	
}
