#include "syscall.h"
#include "copyright.h"
#define nameLen 31
#define idLen 11

int main()
{
	PrintString("Thong tin cac thanh vien trong nhom\n");
	PrintString("Ho ten\t\t\tMSSV\n");
	PrintString("Tran Xuan Quy\t\t18120231\n");
	PrintString("Tran Minh Tri\t\t18120251\n");
	PrintString("Truong Quang Luong\t18120459\n");
	PrintString("Truong Minh Tai\t\t18120544\n");

	PrintString("Gioi thieu ngan gon chuong trinh ascii va sort\n");
	PrintString("Chuong trinh ascii: in ra bang ma ASCII cua cac ky tu doc duoc.\n");
	PrintString("Chuong trinh sort: sap xep mang n so nguyen do nguoi dung nhap vao theo chieu tang hoac giam dan (tuy chon).\n");
	Halt();
	
}
