#ifndef _TCPCLIENTSOCKET_H_
#define _TCPCLIENTSOCKET_H_

#include "NetBuffer.h"

class CTCPClientSocket
{
    public: 
        CTCPClientSocket(){}
        ~CTCPClientSocket(){}
        int RecvBuff();
        int SendBuff();
    private:
        int readfd;
        CNetBuffer recvBuff;
        CNetBuffer sendBuff;
};

#endif
