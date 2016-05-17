#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>
#include "TypeDefine.h"

const int DataBufferSize = 1024;

typedef struct SData
{
    int32_t type;
    int32_t bufferLength;
    Byte buffer[DataBufferSize];
}Data;

#endif
