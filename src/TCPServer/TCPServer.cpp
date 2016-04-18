#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <list>
#include <memory>
#include <unistd.h>

#include "TCPServer.h"
#include "TCPClientSocket.h"
#include "Process.h"

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

int AddListenEvent(int epollfd, int eventfd)
{
    struct epoll_event event;
    event.data.fd = eventfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, eventfd, &event) == -1)
    {
        return -1;
    }
    return 0;
}

int RemoveListenEvent(int epollfd, int eventfd)
{
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, eventfd, NULL) == -1)
    {
        return -1;
    }
    return -1;
}

int AddAcceptEvent(int epollfd, int eventfd)
{
    struct epoll_event event;
    event.data.fd = eventfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, eventfd, &event) == -1)
    {
        return -1;
    }
    std::weak_ptr<CTCPClientSocket> p(ClientSocketManager::GetInstance().CreateClient(eventfd));
    if (p.expired())
    {
        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, eventfd, NULL) == -1)
        {
            return -1;
        }
        return 0;
    }
    return 0;
}

int RemoveAcceptEvent(int epollfd, int eventfd)
{
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, eventfd, NULL) == -1)
    {
        return -1;
    }
    if (ClientSocketManager::GetInstance().DestoryClient(eventfd) == -1)
    {
        return -1;
    }
    return 0;
}

int RecvData(int epollfd, int readfd)
{
    int nread = 0;
    std::weak_ptr<CTCPClientSocket> p(ClientSocketManager::GetInstance().GetClient(readfd));
    if (p.expired())
    {
        return -1;
    }
    std::shared_ptr<CTCPClientSocket> sp = p.lock();
    if (sp == nullptr)
    {
        return -1;
    }
    nread = sp->RecvBuff();
    if (nread > 0)
    {
        Data data;
        if (sp->GetFormatData(data) != 0)
        {
            ProcessHandle::GetInstance().ProcessData(data);
        }
    }
    else
    {
        if (RemoveAcceptEvent(epollfd, readfd) == -1)
        {
            return -1;
        }
    }
    return 0;
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

    if (AddListenEvent(epollfd, listenfd) == -1)
    {
        return -1;
    }
    return 0;
}


int CTCPServer::Accept()
{
    int nfds = 0;
    struct epoll_event get_event[serverMaxEvent];
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
            if (AddAcceptEvent(epollfd, acceptfd) == -1)
            {
                return -1;
            }
        }
        else
        {    if (get_event[i].events & EPOLLIN)
            {
                if (RecvData(epollfd, get_event[i].data.fd) == -1)
                {
                    return -1;
                }
            }
        }
    }
    return 0;
}

