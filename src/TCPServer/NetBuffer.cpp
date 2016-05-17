#include "NetBuffer.h"
#include <string.h>
#include <unistd.h>
#include <stdint.h>
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
    DataMoveToHead();
    int readSize = 0;
    readSize = readfunc(fd, buffer + endPosition, MsgMaxLength - endPosition);
    endPosition += readSize;
    return readSize;
}

int CNetBuffer::GetBuffType(int32_t &type)
{
    return GetStream((Byte *)&type , sizeof(int32_t)/sizeof(Byte));
}

int CNetBuffer::GetBuffLength(int32_t &buffLength)
{
    return GetStream((Byte *)&buffLength , sizeof(int32_t)/sizeof(Byte));
}

int CNetBuffer::GetStream(Byte *buffer, int size)
{
    int Length = endPosition - beginPosition;
    if (Length < 0)
    {
        Clear();
        return 0;
    }
    int ReadSize = (size > Length) ? Length : size;
    memmove(buffer, this->buffer + beginPosition, ReadSize);
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

void CNetBuffer::DataMoveToHead()
{
    if (beginPosition == 0)
    {
        return;
    }
    int Length = endPosition - beginPosition;
    if (Length < 0)
    {
        Clear();
        return;
    }
    memmove(buffer, this->buffer + beginPosition, Length);
    this->beginPosition = 0;
    this->endPosition = Length;
}
