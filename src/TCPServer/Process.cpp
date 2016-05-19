#include "Process.h"

#include <stdio.h>
#include <memory>

#include "Data.h"
#include "../DBServer/DBServer.h"
#include "../TCPServer/TCPClientSocket.h"
#include "../Player/PlayerData.h"
#include "../protobuf/ClientMessage.pb.h"

bool SendCheck(std::weak_ptr<CPlayer> player)
{
    if(player.expired())
    {
        return false;
    }
    std::shared_ptr<CPlayer> playerPtr = player.lock();
    if (playerPtr->GetSocket().expired())
    {
        return false;
    }
    std::shared_ptr<CTCPClientSocket> TCPServer = playerPtr->GetSocket().lock();
    return true;
}

int LoginCheck(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }

    PBC2SLoginCheckReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CLoginCheckRes res;
    if(DBServer::GetInstance().LoginCheck(req.username().c_str(), req.password().c_str(), res) != 0)
    {
        return -1;
    }

    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CLoginCheckRes, res);

    return 0;
}

int LoginRegister(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBC2SLoginRegisterReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CLoginRegisterRes res;
    if (DBServer::GetInstance().LoginRegister(req.username().c_str(), req.password().c_str(), res) != 0)
    {
        return -1;
    }
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CLoginRegisterRes, res);
    return 0;
}

int CreateUser(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBC2SCreateUserReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CCreateUserRes res;
    if (DBServer::GetInstance().CreateUser(player.lock()->GetPlayerID(), req.playername().c_str(), res) != 0)
    {
        return -1;
    }

    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CCreateUserRes, res);
    return 0;
}

int DeleteUser(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBC2SDeleteUserReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CDeleteUserRes res;
    if (DBServer::GetInstance().DeleteUser(player.lock()->GetPlayerID(), res) != 0)
    {
        return -1;
    }

    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CDeleteUserRes, res);
    return 0;
}

int GetPlayerInfo(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBS2CGetPlayerInfoRes res;
    if (player.lock()->PlayerInfo2PB((*res.mutable_playerinfo())))
    {
        return -1;
    }
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetPlayerInfoRes, res);
    return 0;
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

int GetHeroTeam(const Data& data, std::weak_ptr<CPlayer> player)
{
    return 0;
}

int UpdateHeroTeam(const Data& data, std::weak_ptr<CPlayer> player)
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

