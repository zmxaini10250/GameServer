#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <unordered_map>

#include "Data.h"
#include "../ObjectPool/SingletonObject.hpp"

class CPlayer;

typedef int(*ProcessFunction)(const Data& data, std::weak_ptr<CPlayer> player);

class CProcess
{
    public:
        int ProcessData(const Data& data, std::weak_ptr<CPlayer> player);
        int RegisterFunction(int type, ProcessFunction function);
    protected:
        typedef std::unordered_map<int, ProcessFunction> FunctionList;
        FunctionList list;
        CProcess();
        ~CProcess(){}
};

typedef CSingletonObject<CProcess> ProcessHandle;

#endif

