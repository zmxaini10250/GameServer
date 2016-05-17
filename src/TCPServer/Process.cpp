#include "Process.h"

#include <stdio.h>

#include "Data.h"
#include "../protobuf/login.pb.h"
#include "../DBServer/DBServer.h"
#include "../protobuf/messageType.pb.h"

int LoginCheck(const Data& data, std::weak_ptr<CPlayer> player)
{
    PBC2SLoginReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    printf("username:%s password:%s\n", req.username().c_str(), req.password().c_str());

    int result = DBServer::GetInstance().LoginCheck(req.username().c_str(), req.password().c_str()); 
    printf("result:%d\n", result);
    return 0;
}

CProcess::CProcess()
{
    RegisterFunction((int)TypeC2SLoginReq, LoginCheck);
}

int CProcess::ProcessData(const Data& data, std::weak_ptr<CPlayer> player)
{

    FunctionList::const_iterator it = list.find(data.type);
    if (it == list.cend())
    {
        return -1;
    }
    (it->second)(data, player);
    return 0;
}

int CProcess::RegisterFunction(int type, ProcessFunction function)
{
    FunctionList::const_iterator it = list.find(type);
    if (it != list.cend())
    {
        return -1;
    }
    list.insert(std::make_pair(type, function));
    return 0;
}


