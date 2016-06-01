#include "TCPClientSocket.h"

#include <memory>
#include <map>
#include <sys/socket.h>
#include <google/protobuf/message.h>
#include <google/protobuf/wire_format.h>

#include "Data.h"
#include "NetBuffer.h"
#include "../Player/PlayerData.h"
#include "../ObjectPool/ObjectPool.hpp"
#include "../protobuf/ClientMessage.pb.h"

int CTCPClientSocket::RecvBuff()
{
    return recvBuff.ReadFromFD(readfd);
}

int CTCPClientSocket::SendBuff(int32_t type, ::google::protobuf::Message& message)
{
    int BuffLength = DataBufferSize + 2 * sizeof(int32_t) + 1;
    Byte Buffer[DataBufferSize + 2 * sizeof(int32_t) + 1];
    int32_t Length = message.ByteSize();
    memset(Buffer, 0, BuffLength);

    memcpy(Buffer, &type, sizeof(int32_t));
    memcpy(Buffer + sizeof(int32_t), &Length, sizeof(int32_t));
    message.SerializePartialToArray(Buffer + 2 * sizeof(int32_t), DataBufferSize);
    if(send(readfd, Buffer, Length + 2 * sizeof(int32_t), 0) == -1)
    {
        return -1;
    }
 
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

void CTCPClientSocket::SetPlayer(std::weak_ptr<CPlayer> player)
{
    this->player = player;
}

void CTCPClientSocket::SetLoginID(int Number)
{
    LoginID = Number;
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
