#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

//#include "define.h"
#include "const_value.h"

class CTCPServer
{
    public:
        CTCPServer(int port = serverPort, int waiNumber = serverWaitNumber);
        ~CTCPServer();
        int Listen();
        int Accept();
    private:
        int listenfd;
        int listenPort;
        int listenWaitNumber;
};

#endif
