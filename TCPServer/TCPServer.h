#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <list>
#include <memory>
#include "NetBuffer.h"

class CTCPServer
{
    public:
        CTCPServer();
        ~CTCPServer(){}
        int Listen();
        int Accept();
        int GetRecvBuff(Byte *buff, int size);
    private:
        //int AddRecvBuff(){}
        //int AddSendBuff(){}
        //int GetSendBuff(){}
    private:
        int epollfd;
        int listenfd;
    
        std::list<std::unique_ptr<CNetBuffer> > freeSpace;
        std::list<std::unique_ptr<CNetBuffer> > sendSpace;
        std::list<std::unique_ptr<CNetBuffer> > recvSpace;

};

#endif
