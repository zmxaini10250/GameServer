#ifndef _DATA_H_
#define _DATA_H_

#include "TypeDefine.h"

const int DataBufferSize = 1024;

typedef struct SData
{
    Byte buffer[DataBufferSize];
}Data;

#endif
