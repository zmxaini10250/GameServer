#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "Data.h"
#include "../ObjectPool/SingletonObject.hpp"


class CProcess
{
    public:
        void ProcessData(const Data& data);
    protected:
        CProcess(){}
        ~CProcess(){}
};

typedef CSingletonObject<CProcess> ProcessHandle;

#endif
