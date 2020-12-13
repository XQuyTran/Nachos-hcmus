#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

class PCB
{
private:
	Semaphore	*joinsem;	//semaphore cho qua trinh join
	Semaphore	*exitsem;	//semaphore cho qua trinh exit
	Semaphore	*mutex;
	int		exitcode;
	Thread		*thread;
	int		pid;
	int		numwait;	//so tien trinh da join

public:
	int 		parentID;	//ID cua tien trinh cha
	int		JoinStatus;	//Trang thai co Join voi tien trinh nao khong? neu co thi gia tri chinh la ID cua tien trinh ma no Join

	PCB(int id);
	~PCB();
	int Exec(char *filename, int pID); //nap chuong trinh co ten luu trong bien filename va processID se la pID
	int GetID();
	int GetNumWait();
	void JoinWait();
	void ExitWait();
	void JoinRelease();
	void ExitRelease();
	void IncNumWait();
	void DecNumWait();
	void SetExitCode(int ec);
	int GetExitCode();
	char* GetNameThread();
};

//*********************************************************************************************************************
void MyStartProcess(int pID);

#endif