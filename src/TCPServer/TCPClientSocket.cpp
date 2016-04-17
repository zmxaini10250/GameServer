#include "TCPClientSocket.h"
#include "NetBuffer.h"
#include "../ObjectPool/ObjectPool.hpp"

int CTCPClientSocket::RecvBuff()
{
    recvBuff.ReadFromFD(readfd);
    return 0;
}

int CTCPClientSocket::SendBuff()
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////////

std::weak_ptr<CTCPClientSocket> CTCPClientSocketManager::CreateClient(int readfd)
{
    ClientList::const_iterator it = list.find(readfd);
    if (it != list.end())
    {
        return std::weak_ptr<CTCPClientSocket>();
    }
    ClientSocketPool::Ptr p = ClientSocketPool::CreateObject(readfd);
    list.insert.(std::make_pair<int, ClientSocketPool::Ptr>(readfd, p));
    return std::weak_ptr<CTCPClientSocket>(p);
}
