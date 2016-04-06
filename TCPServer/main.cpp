#include "TCPServer.h"

int main()
{
    CTCPServer server;
    server.Listen();
    server.Accept();
    return 0;
}
