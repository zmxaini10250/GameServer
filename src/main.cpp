#include <iostream>
#include "TCPServer/TCPServer.h"

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
