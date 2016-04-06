#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
int main()
{
    int listenfd;
    int nread;
    struct sockaddr_in servaddr;
    char readbuf[256];


    printf("socket started\n");
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket error\n");
        return -1;
    }

    if (fcntl(listenfd, F_SETFL, O_NONBLOCK) == -1)
    {
        printf("set listen fcntl failed\n");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(59000);

    printf("bind started\n");
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind error\n");
        return -1;
    }

    printf("listen started\n");
    if (listen(listenfd, 1) == -1)
    {
        printf("listen error\n");
        return -1;
    }
    
    int epoll_fd;
    const int max_event = 128;

    int timeout = 10000;
    epoll_fd = epoll_create(max_event);
    if(epoll_fd == -1)
    {
        printf("epoll create error\n");
        return -1;
    }
    struct epoll_event listenevent;
    listenevent.data.fd = listenfd;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &listenevent) == -1)
    {
        printf("epoll control error\n");
        return -1;
    }
    
    int nfds = 0;
    struct epoll_event get_event[max_event];

    for (;;)
    {
        memset(get_event, 0 ,sizeof(epoll_event) * max_event);
        nfds = epoll_wait(epoll_fd, get_event, max_event, timeout);
        for (int i = 0; i < nfds; ++i)
        {
            if (get_event[i].data.fd == listenfd)
            {

                printf("accept start\n");
                int acceptfd = 0;
                if ((acceptfd = accept(listenfd, NULL, NULL)) < 0)
                {
                    printf("accept error\n");
                    return -1;
                }
                struct epoll_event tem_event;
                if (fcntl(acceptfd, F_SETFL, O_NONBLOCK) == -1)
                {
                    printf("set accept fcntl error\n");
                    return -1;
                }

                //tem_event.events |= EPOLLOUT;
                tem_event.data.fd = acceptfd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, acceptfd, &tem_event);
            }
            else
            {
                if (get_event[i].events & EPOLLIN)
                {
                    printf("recv or close start\n");
                    memset(readbuf, 0, sizeof(readbuf));
                    nread = read(get_event[i].data.fd, readbuf, sizeof(readbuf));
                    if (nread > 0)
                    {
                        printf("received %s, nread = %d\n\n", readbuf, nread);
                    }
                    else
                    {
                        if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, get_event[i].data.fd, NULL) == -1)
                        {
                            printf("event remove failed\n");
                            return -1;
                        }
                        printf("client close!\n");
                    }
                }
            }

        }
        
        printf("loop\n");
    }
    return 0;
}
