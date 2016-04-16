#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
int main()
{
    int count = 0;
    int listenfd, socketfd;
    int nread;
    struct sockaddr_in servaddr;
    char readbuf[256];


    printf("socket started\n");
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket error\n");
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

    printf("accept started\n");
    if ((socketfd = accept(listenfd, NULL, NULL)) < 0 )
    {
        printf("accept error\n");
        return -1;
    }


    while(1) 
    {
        printf("start receive %d...\n", count++);

        memset(readbuf, sizeof(readbuf), 0);
        nread = recv(socketfd, readbuf, 10, 0);
        if(nread>0)
        {
            readbuf[nread+1] = '\0';
            printf("receiveed %s, nread = %d\n\n", readbuf, nread);
        }
        else
        {
            break;
        }
    }

    return 0;
}
