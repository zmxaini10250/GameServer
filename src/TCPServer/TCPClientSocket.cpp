#include <memory>
#include <map>

#include "Data.h"
#include "TCPClientSocket.h"
#include "NetBuffer.h"
#include "../ObjectPool/ObjectPool.hpp"

int CTCPClientSocket::RecvBuff()
{
    return recvBuff.ReadFromFD(readfd);
}

int CTCPClientSocket::SendBuff()
{
    return 0;
}

int CTCPClientSocket::GetFormatData(Data &data)
{
    return recvBuff.GetStream(data.buffer, DataBufferSize);
}

//////////////////////////////////////////////////////////////////////////////

std::weak_ptr<CTCPClientSocket> CTCPClientSocketManager::CreateClient(int readfd)
{
    ClientList::const_iterator it = list.find(readfd);
    if (it != list.end())
    {
        return std::weak_ptr<CTCPClientSocket>();
    }
    ClientSocketPool::Ptr p(ClientSocketPool::GetInstance().CreateObject(readfd));
    if (p != nullptr)
    {
        list.insert(std::make_pair(readfd, p));
    }
    return std::weak_ptr<CTCPClientSocket>(p);
}

std::weak_ptr<CTCPClientSocket> CTCPClientSocketManager::GetClient(int readfd)
{
    ClientList::const_iterator it = list.find(readfd);
    if (it == list.end())
    {
        return std::weak_ptr<CTCPClientSocket>();
    }
    return std::weak_ptr<CTCPClientSocket>(it->second);   
}


int CTCPClientSocketManager::DestoryClient(int readfd)
{
    list.erase(readfd);
    return 0;
}
