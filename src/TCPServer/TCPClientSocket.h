#ifndef _TCPCLIENTSOCKET_H_
#define _TCPCLIENTSOCKET_H_

#include <memory>
#include <unordered_map>
#include "NetBuffer.h"
#include "../ObjectPool/ObjectPool.hpp"
#include "../ObjectPool/SingletonObject.hpp"
#include "Data.h"
#include "../Player/PlayerData.h"

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
        std::weak_ptr<CPlayer> GetPlayer(){ return player; }
        int GetFormatData(Data &data);
    private:
        std::weak_ptr<CPlayer> player;
        int readfd;
        CNetBuffer recvBuff;
        

};

class CTCPClientSocketManager
{
    public:
        std::weak_ptr<CTCPClientSocket> CreateClient(int readfd);
        std::weak_ptr<CTCPClientSocket> GetClient(int readfd);
        int DestoryClient(int readfd);
    protected:
        typedef std::unordered_map<int, ClientSocketPool::Ptr> ClientList;
        ClientList list;
        CTCPClientSocketManager(){}
        ~CTCPClientSocketManager(){}
};


#endif
