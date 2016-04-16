#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_


class CTCPServer
{
    public:
        CTCPServer();
        ~CTCPServer(){}
        int Listen();
        int Accept();
    private:
        int epollfd;
        int listenfd;
};

#endif
