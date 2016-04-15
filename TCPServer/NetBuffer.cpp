#include "NetBuffer.h"
#include <string.h>

CNetBuffer::CNetBuffer():beginPosition(0), endPosition(0)
{
    memset(buffer, 0, MsgMaxLength);   
}

void CNetBuffer::Clear()
{
    beginPosition = endPosition = 0;
    memset(buffer, 0, MsgMaxLength);
}

int CNetBuffer::ReadFromFD(int fd, ReadFunction readfunc)
{
    int readSize = 0;
    readSize = readfunc(fd, buffer + endPosition, MsgMaxLength - endPosition);
    endPosition += readSize;
    return endPosition;
}

int CNetBuffer::GetStream(Byte *buffer, int size)
{
    int Length = endPosition - beginPosition;
    if (Length < 0)
    {
        Clear();
    }
    int ReadSize = (size > Length) ? Length : size;
    memmove(buffer, this->buffer, ReadSize);
    beginPosition += ReadSize;
    return ReadSize;
}

bool CNetBuffer::isFull()
{
    return endPosition == MsgMaxLength;
}

bool CNetBuffer::isEmpty()
{
    return endPosition == beginPosition;
}
