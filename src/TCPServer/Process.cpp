#include "Process.h"

#include <stdio.h>
#include <memory>


#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#include "../LuaTinker/lua_tinker.h"

#include "Data.h"
#include "../DBServer/DBServer.h"
#include "../TCPServer/TCPClientSocket.h"
#include "../Player/PlayerData.h"
#include "../protobuf/ClientMessage.pb.h"
#include "../Hero/Hero.h"
#include "../Consume/Consume.h"
#include "../Drop/Drop.h"


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
    if (SendCheck(player))
    {
        return -1;
    }
    PBS2CGetHeroListRes res;
    if (player.lock()->GetHeroData().GetHeroPack().HeroList2PB(res) != 0)
    {
        return -1;
    }
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetHeroListRes, res);
    return 0;
}

int GetHeroInfo(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBC2SGetHeroInfoReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    
    std::weak_ptr<CHero> p = player.lock()->GetHeroData().GetHeroPack().GetHero(req.heroindex());
    if (p.expired())
    {
        return -1;
    }

    PBS2CGetHeroInfoRes res;

    p.lock()->HeroInfo2PB(*res.mutable_heroinfo());

    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetHeroInfoRes, res);
    return 0;
}

int HeroLevelUp(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBC2SHeroLevelUpReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    std::weak_ptr<CHero> p = player.lock()->GetHeroData().GetHeroPack().GetHero(req.heroindex());
    if (p.expired())
    {
        return -1;
    }
    CConsume consume((int)ConsumeNumerical, (int)ConsumeEmpirical, p.lock()->GetUpLevelEmpirical(), *player.lock());
    if (!consume.CheckConsume())
    {
        return -1;
    }
    consume.ConsumeReduce();

    PBS2CHeroLevelUpRes res;

    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CHeroLevelUpRes, res);
    return 0;
}

int DeleteHero(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBC2SDeleteHeroReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CDeleteHeroRes res;
    player.lock()->GetHeroData().GetHeroPack().RemoveHero(req.heroindex());
    
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CDeleteHeroRes, res);
    return 0;
}

int GetHeroTeam(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBS2CGetHeroTeamRes res;
    player.lock()->GetHeroData().HeroTeam2PB(*res.mutable_heroteam());
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetHeroTeamRes, res);
    return 0;
}

int UpdateHeroTeam(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBC2SUpdateHeroTeamReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CUpdateHeroTeamRes res;
    player.lock()->GetHeroData().GetHeroTeam().PB2Team(req);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CUpdateHeroTeamRes, res);
    return 0;
}

int LotteryLogic(CPlayer& player)
{
	lua_State* L = lua_open();
	luaopen_base(L);
	lua_tinker::dofile(L, "../LuaCfg/Lottery.lua");

	int ConsumeNumber = lua_tinker::get<int>(L, "LotteryConsume");
    int DropNumber = lua_tinker::get<int>(L, "LotteryDrop");

    lua_close(L);

    CConsume consume((int)ConsumeNumerical, (int)ConsumeGold, ConsumeNumber, player);
    if (!consume.CheckConsume())
    {
        return -1;
    }
    CDrop drop(DropNumber, (int)DropNonCumulative, (int)DropHero, player);
    if (!drop.CheckPack())
    {
        return -1;
    }
    consume.ConsumeReduce();
    drop.DropAdd();
    return 0;
}

int Lottery(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
    PBS2CLotteryRes res;
    LotteryLogic(*player.lock());
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CLotteryRes, res);
    return 0;
}

int GetPlayerList(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }

    return 0;
}

int PlayerFight(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (SendCheck(player))
    {
        return -1;
    }
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

