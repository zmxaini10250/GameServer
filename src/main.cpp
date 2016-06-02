#include "TCPServer/TCPServer.h"
#include "Timer/Timer.h"
#include "DBServer/DBServer.h"
#include <stdio.h>
int main()
{
    CTCPServer server;
    if (server.Listen() == -1)
    {
        printf("network error\n");
    }
    
    for(;;)
    {
        server.Accept();
    }
	return 0;
}
