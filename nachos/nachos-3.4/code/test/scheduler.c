#include "syscall.h"

int main()
{
	int pingID, pongID;
	PrintString("Ping-Pong test starting ...\n\n");
	pingID = Exec("./test/ping");
	pongID = Exec("./test/pong");	
	Join(pingID);
	Join(pongID);
	PrintString("\n\n");
}
