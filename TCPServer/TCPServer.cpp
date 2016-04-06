#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include "TCPServer.h"

CTCPServer::CTCPServer(const int port, const int waitNumber):listenPort(port),listenWaitNumber(waitNumber)
{
}

CTCPServer::~CTCPServer()
{
}

int CTCPServer::Listen()
{
    struct sockaddr_in serverAddr;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return -1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(listenPort);

    if(bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(sockaddr_in)) < 0)
    {
        return -1;
    }
    
    if (listen(listenfd, listenWaitNumber) < 0)
    {
        return -1;
    }
    return 0;
}


int CTCPServer::Accept()
{
    int socketfd = 0;
    char readbuf[256];
    int nread = 0;

    socketfd = accept(listenfd, NULL, NULL);
    while(1) 
    {

        memset(readbuf, sizeof(readbuf), 0);
        nread = recv(socketfd, readbuf, 10, 0);
        if(nread>0)
        {
            readbuf[nread+1] = '\0';
        }
        else
        {
            break;
        }
    }


    return 0;
}

