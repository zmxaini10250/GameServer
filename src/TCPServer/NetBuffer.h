#ifndef _NETBUFFER_H_
#define _NETBUFFER_H_

#define MsgMaxLength (1024)

#include <unistd.h>
#include <stdint.h>
#include "TypeDefine.h"

class CNetBuffer
{
    public:
        CNetBuffer();
        virtual ~CNetBuffer(){}
        int ReadFromFD(int fd, ReadFunction readfunc = read);
        int GetBuffType(int32_t type);
        int GetBuffLength(int32_t buffLength);
        int GetStream(Byte *buffer, int size);
        bool isFull();
        bool isEmpty();
    private:
        void Clear();
    private:
        void DataMoveToHead();
        int beginPosition;
        int endPosition;
        Byte buffer[MsgMaxLength];
};

#endif
