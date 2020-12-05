#include "syscall.h"
#include "copyright.h"
#define nameLen 31
#define idLen 11

int main()
{
	int nameLen = 31;
	int idLen = 11;
	int i;

	char name1[namelen] = "Truong Minh Tai";
	char name2[namelen] = "Tran Xuan Quy";
	char name3[namelen] = "Truong Quang Luong";
	char name4[namelen] = "Tran Minh Tri";
	char* names[4] = {name1, name2, name3, name4};
	
	char id1[idLen] = "18120544";
	char id2[idLen] = "18120231";
	char id3[idLen] = "18120459";
	char id4[idLen] = "18120251";
	char* ids[4] = {id1,id2,id3,id4};

	PrintString("Thong tin thanh vien trong nhom\n");
	PrintString("Ho ten\tMSSV\n");

	for (i = 0; i < 4; i++)
	{
		PrintString(names[i]);
		PrintString("\t");
		PrintString(id[i]);
		PrintString("\n");
	}

	PrintString("Chuong trinh ascii: in ra bang ma ASCII cua cac ky tu doc duoc.\n");
	PrintString("Chuong trinh sort: sap xep mang n so nguyen do nguoi dung nhap vao theo chieu tang hoac giam dan (tuy chon).\n");
	Halt();
	
}
