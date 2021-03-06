#ifndef _TCPCLIENTSOCKET_H_
#define _TCPCLIENTSOCKET_H_

#include <memory>
#include <unordered_map>
#include <google/protobuf/message.h>
#include <google/protobuf/wire_format.h>
#include "NetBuffer.h"
#include "../ObjectPool/ObjectPool.hpp"
#include "../ObjectPool/SingletonObject.hpp"
#include "Data.h"

class CPlayer;

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
        int SendBuff(int32_t type, ::google::protobuf::Message& message);
        std::weak_ptr<CPlayer> GetPlayer(){ return player; }
        int GetFormatData(Data &data);
        void SetPlayer(std::weak_ptr<CPlayer> player);
        int GetReadFD(){ return readfd; }
        void SetLoginID(int Number);
        int GetLoginID(){ return LoginID; }
    private:
        std::weak_ptr<CPlayer> player;
        int readfd;
        int LoginID;
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
