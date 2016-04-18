#ifndef _NETBUFFER_H_
#define _NETBUFFER_H_

#define MsgMaxLength (1024)

#include <unistd.h>
#include "TypeDefine.h"

enum eMsgType
{
    Normal,
    MsgTypeEnd
};

class CNetBuffer
{
    public:
        CNetBuffer();
        virtual ~CNetBuffer(){}
        int ReadFromFD(int fd, ReadFunction readfunc = read);
        int GetStream(Byte *buffer, int size);
        bool isFull();
        bool isEmpty();
    private:
        void Clear();
    private:
        int beginPosition;
        int endPosition;
        Byte buffer[MsgMaxLength];
};

#endif
