#include "TCPServer/TCPServer.h"
#include "Timer/Timer.h"
#include "DBServer/DBServer.h"
int main()
{
    CTCPServer server;
    server.Listen();
    
    for(;;)
    {
        server.Accept();
    }
	return 0;
}
