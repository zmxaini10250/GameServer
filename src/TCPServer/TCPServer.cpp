#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <list>
#include <memory>
#include <unistd.h>

#include "TCPServer.h"
#include "NetBuffer.h"

const int MsgListMaxLength  = 1024;
const int serverPort        = 59000;
const int serverWaitNumber  = 1024;
const int serverMaxEvent    = 1024;
const int serverWaitTime    = 500;

int SetNoBlock(int fd)
{
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
    {
        return -1;
    }
    return 0;
}

int AddEvent(int epollfd, int eventfd)
{
    struct epoll_event event;
    event.data.fd = eventfd;
    
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, eventfd, &event) == -1)
    {
        return -1;
    }
    return 0;
}

int RemoveEvent(int epollfd, int eventfd)
{
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, eventfd, NULL) == -1)
    {
        return -1;
    }
    return -1;
}

CTCPServer::CTCPServer()
{
}

int CTCPServer::Listen()
{
    struct sockaddr_in serverAddr;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return -1;
    }
    
    if (SetNoBlock(listenfd) == -1)
    {
        return -1;
    }


    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    
    if (bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(sockaddr_in)) < 0)
    {
        return -1;
    }
    
    if (listen(listenfd, serverWaitNumber) < 0)
    {
        return -1;
    }

    epollfd = epoll_create(serverMaxEvent);
    if (epollfd == -1)
    {
        printf("epoll create error\n");
        return -1;
    }

    if (AddEvent(epollfd, listenfd) == -1)
    {
        return -1;
    }
    return 0;
}


int CTCPServer::Accept()
{
    int nfds = 0;
    struct epoll_event get_event[serverMaxEvent];
    int nread = 0;
    memset(get_event, 0, sizeof(epoll_event) * serverMaxEvent);
    nfds = epoll_wait(epollfd, get_event, serverMaxEvent, serverWaitTime);
    for (int i = 0; i < nfds; ++i)
    {
        if (get_event[i].data.fd == listenfd)
        {
            //TODO Function
            int acceptfd = 0;
            if ((acceptfd = accept(listenfd, NULL, NULL)) < 0)
            {
                return -1;
            }
            if (SetNoBlock(acceptfd) == -1)
            {
                return -1;
            }
            if (AddEvent(epollfd, acceptfd) == -1)
            {
                return -1;
            }
        }
        else
        {
            //TODO Function
            if (get_event[i].events & EPOLLIN)
            {
                if (freeSpace.empty())
                {
                    return 0;
                }
                auto it = freeSpace.begin();
                nread = (*it)->ReadFromFD(get_event[i].data.fd);
                if (nread > 0)
                {
                    recvSpace.push_back(std::move((*it)));
                    freeSpace.pop_front();
                }
                else
                {
                    if (RemoveEvent(epollfd, get_event[i].data.fd) == -1)
                    {
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}

int CTCPServer::GetRecvBuff(Byte *buff, int size)
{
    if (recvSpace.empty())
    {
        return -1;
    }
    recvSpace.front()->GetStream(buff, size);
    freeSpace.push_back(std::move(recvSpace.front()));
    recvSpace.pop_front();
    return 0;
}

