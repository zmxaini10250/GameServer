#ifndef _TCPCLIENTSOCKET_H_
#define _TCPCLIENTSOCKET_H_

#include <memory>
#include <map>
#include "NetBuffer.h"
#include "../ObjectPool/ObjectPool.hpp"
#include "../ObjectPool/SingletonObject.hpp"

typedef Data;
class CTCPClientSocket;
class CTCPClientSocketManager;

typedef CObjectPool<CTCPClientSocket> ClientSocketPool;
typedef CSingletonObject<CTCPClientSocketManager> ClientSocketManager;


class CTCPClientSocket
{
    public: 
        CTCPClientSocket(int readfd): readfd(readfd){}
        ~CTCPClientSocket(){}
        int RecvBuff();
        int SendBuff();
        int GetFormatData(Data &data);
    private:
        int readfd;
        CNetBuffer recvBuff;
        CNetBuffer sendBuff;
};

class CTCPClientSocketManager
{
    public:
        std::weak_ptr<CTCPClientSocket> CreateClient(int readfd);
    protected:
        typedef std::map<int, ClientSocketPool::Ptr> ClientList;
        ClientList list;
        CTCPClientSocketManager(){}
        ~CTCPClientSocketManager(){}
};


#endif
