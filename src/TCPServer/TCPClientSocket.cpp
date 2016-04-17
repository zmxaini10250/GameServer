#include "TCPClientSocket.h"
#include "NetBuffer.h"

int CTCPClientSocket::RecvBuff()
{
    recvBuff.ReadFromFD(readfd);
    return 0;
}

int CTCPClientSocket::SendBuff()
{
    return 0;
}


