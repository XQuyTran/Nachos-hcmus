#include "syscall.h"
#include "copyright.h"

int ascending(int a, int b) {
	if (a > b)
		return 1;
	return 0;
}

int descending(int a, int b) {
	if (a < b)
		return 1;
	return 0;
}

// ham sap xep cac phan tu cua mang theo thuat toan Bubble Sort voi con tro ham
void bubbleSort(int arr[], int n, int (*comparisonFunc)(int, int)) {
	int i, j, temp, haveSwap;
	for (i = 0; i < n - 1; i++) {
		haveSwap = 0;
		for (j = 0; j < n - i - 1; j++) {
			if (comparisonFunc(arr[j], arr[j + 1])) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				haveSwap = 1;
			}
		}
		if (haveSwap == 0)
			break;
	}
}


// In cac phan tu cua mang
void printArr(int arr[], int n) {
	int i;
	for (i = 0; i < n; i++) {
		PrintInt(arr[i]);
		if (i != n - 1)
			PrintString(", ");
	}
}

int main()
{
	int n, i, j, temp;
	int arr[100];
	int choice, haveSwap;
	do {
		PrintString("Nhap so phan tu cua mang: ");
		n = ReadInt();
	} while (n < 1 || n > 100);

	PrintString("Nhap cac phan tu cua mang: \n");
	for (i = 0; i < n; i++) {
		PrintString("a[");
		PrintInt(i);
		PrintString("]= ");
		arr[i] = ReadInt();	// Neu nhap khong hop le thi a[i] = 0
	}
	
	PrintString("Cac phan tu mang la: ");
	printArr(arr, n);

	do {
		PrintString("\n\nBan muon sap xep tang dan hay giam dan(tang(1)/giam(2))???");
		PrintString("\nBan chon[1/2]: ");
		choice = ReadInt();
	} while (choice != 1 && choice != 2);

	if (choice == 1) {
		bubbleSort(arr, n, ascending);
	} 
	else {
		bubbleSort(arr, n, descending);
	}

	PrintString("Cac phan tu sau khi da duoc sap xep la: ");
	printArr(arr, n);
	return 1;
}



