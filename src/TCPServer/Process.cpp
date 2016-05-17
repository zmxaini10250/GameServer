#include "Process.h"

#include <stdio.h>

#include "Data.h"
#include "../DBServer/DBServer.h"

#include "../protobuf/ClientMessage.pb.h"

int LoginCheck(const Data& data, std::weak_ptr<CPlayer> player)
{
    PBC2SLoginCheckReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    printf("username:%s password:%s\n", req.username().c_str(), req.password().c_str());

    int result = DBServer::GetInstance().LoginCheck(req.username().c_str(), req.password().c_str()); 
    printf("result:%d\n", result);
    return 0;
}

int LoginRegister(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int CreateUser(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int DeleteUser(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int GetPlayerInfo(const Data& data, std::weak_ptr<CPlayer> player)
{
    return -1;
}

int GetHeroList(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int GetHeroInfo(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int HeroLevelUp(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int DeleteHero(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int GetTeam(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int UpdateTeam(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int Lottery(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int GetPlayerList(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int PlayerFight(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

CProcess::CProcess()
{
    RegisterFunction((int)TypeC2SLoginCheckReq, LoginCheck);
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

