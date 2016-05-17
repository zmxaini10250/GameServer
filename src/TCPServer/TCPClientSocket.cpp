#include <memory>
#include <map>

#include "Data.h"
#include "TCPClientSocket.h"
#include "NetBuffer.h"
#include "../ObjectPool/ObjectPool.hpp"
#include "../protobuf/Message.pb.h"

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
    if (recvBuff.GetBuffType(data.type) != sizeof(int32_t) || data.type > (int32_t)eMessageType_MAX)
    {
        return -1;
    }
    if (recvBuff.GetBuffLength(data.bufferLength) != sizeof(int32_t) || (data.bufferLength > DataBufferSize))
    {
        return -1;
    }
    if (recvBuff.GetStream(data.buffer, data.bufferLength) != data.bufferLength)
    {
        return -1;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////

std::weak_ptr<CTCPClientSocket> CTCPClientSocketManager::CreateClient(int readfd)
{
    ClientList::const_iterator it = list.find(readfd);
    if (it != list.cend())
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
    if (it == list.cend())
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
