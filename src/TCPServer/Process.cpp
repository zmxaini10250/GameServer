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
    int i = playerPtr->GetPlayerID();
    if (playerPtr->GetPlayerID() == 0 || playerPtr->GetSocket().expired())
    {
        return false;
    }
    std::shared_ptr<CTCPClientSocket> ClientSocket = playerPtr->GetSocket().lock();
    if (ClientSocket->GetPlayer().expired())
    {
        return false;
    }
    return true;
}

void SavePlayer2DB(std::weak_ptr<CPlayer> player)
{
    if (!player.expired()&&player.lock()->GetPlayerID()!=0)
    {
        DBPlayer oldPlayerData;
        player.lock()->Player2DB(oldPlayerData);
        DBServer::GetInstance().SavePlayerData(oldPlayerData);
    }
}

int LoginCheck(const Data& data, std::weak_ptr<CPlayer> player)
{
    if(player.expired())
    {
        return -1;
    }
    std::shared_ptr<CPlayer> playerPtr = player.lock();
    if (playerPtr->GetSocket().expired())
    {
        return -1;
    }

    PBC2SLoginCheckReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CLoginCheckRes res;
    if (DBServer::GetInstance().LoginCheck(req.username().c_str(), req.password().c_str(), res) != 0)
    {
        return -1;
    }
    if (res.logincheckresult() > 0)
    {
        player.lock()->GetSocket().lock()->SetLoginID(res.logincheckresult());
        DBPlayer playerData;
        if (DBServer::GetInstance().GetPlayerData(res.logincheckresult(), playerData) == 0)
        {
            std::weak_ptr<CTCPClientSocket> socket = player.lock()->GetSocket();
            std::weak_ptr<CPlayer> newPlayer = socket.lock()->GetPlayer();
            if (!newPlayer.expired())
            {
                DBPlayer oldPlayerData;
                player.lock()->Player2DB(oldPlayerData);
                DBServer::GetInstance().SavePlayerData(oldPlayerData);
                socket.lock()->SetPlayer(std::weak_ptr<CPlayer>());
                PlayerManager::GetInstance().RemovePlayer(player.lock()->GetPlayerID());
            }
            newPlayer = PlayerManager::GetInstance().CreatePlayer(playerData.playerid(), socket);
            if (newPlayer.expired())
            {
                return -1;
            }
            newPlayer.lock()->DB2Player(playerData);
            newPlayer.lock()->SetSocket(socket);
            socket.lock()->SetPlayer(newPlayer);
        }

    }
    else
    {
        player.lock()->GetSocket().lock()->SetPlayer(std::weak_ptr<CPlayer>());
    }
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CLoginCheckRes, res);

    return 0;
}

int LoginRegister(const Data& data, std::weak_ptr<CPlayer> player)
{
    if(player.expired())
    {
        return -1;
    }
    std::shared_ptr<CPlayer> playerPtr = player.lock();
    if (playerPtr->GetSocket().expired())
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
    if(player.expired())
    {
        return -1;
    }
    std::shared_ptr<CPlayer> playerPtr = player.lock();
    if (playerPtr->GetSocket().expired())
    {
        return -1;
    }
    if (!playerPtr->GetSocket().lock()->GetPlayer().expired())
    {
        PBS2CCreateUserRes res;
        res.set_createuserresult(-1);
        player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CCreateUserRes, res);
        return 0;
    }

    PBC2SCreateUserReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CCreateUserRes res;
    if (DBServer::GetInstance().CreateUser(player.lock()->GetSocket().lock()->GetLoginID(), req.playername().c_str(), res) != 0)
    {
        return -1;
    }
    DBPlayer playerData;
    if (DBServer::GetInstance().GetPlayerData(playerPtr->GetSocket().lock()->GetLoginID(), playerData) == 0)
    {
        std::weak_ptr<CTCPClientSocket> socket = player.lock()->GetSocket();
        std::weak_ptr<CPlayer> newPlayer = socket.lock()->GetPlayer();
        if (newPlayer.expired())
        {
            newPlayer = PlayerManager::GetInstance().CreatePlayer(playerData.playerid(), socket);
            if (newPlayer.expired())
            {
                return -1;
            }
            newPlayer.lock()->DB2Player(playerData);
            newPlayer.lock()->SetSocket(socket);
            socket.lock()->SetPlayer(newPlayer);
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CCreateUserRes, res);
    return 0;
}

int DeleteUser(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
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
    if (res.deleteuserresult() == 1)
    {
        player.lock()->GetSocket().lock()->SetPlayer(std::weak_ptr<CPlayer>());
        PlayerManager::GetInstance().RemovePlayer(player.lock()->GetPlayerID());
    }
    return 0;
}

int GetPlayerInfo(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBS2CGetPlayerInfoRes res;
    int result = player.lock()->PlayerInfo2PB((*res.mutable_playerinfo()));
    res.set_getplayerinforesult(result);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetPlayerInfoRes, res);
    return 0;
}

int GetHeroList(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBS2CGetHeroListRes res;
    if (player.lock()->GetHeroData().GetHeroPack().HeroList2PB(res) != 0)
    {
        return -1;
    }
    res.set_getherolistresult(0);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetHeroListRes, res);
    return 0;
}

int GetHeroInfo(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
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
    res.set_getheroinforesult(0);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetHeroInfoRes, res);
    return 0;
}

int HeroLevelUp(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    int result = 0;
    PBC2SHeroLevelUpReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    std::weak_ptr<CHero> p = player.lock()->GetHeroData().GetHeroPack().GetHero(req.heroindex());
    if (p.expired())
    {
        return -1;
    }
    CConsume consume(p.lock()->GetUpLevelEmpirical(), (int)ConsumeNumerical, (int)ConsumeEmpirical, *player.lock());
    if (!consume.CheckConsume())
    {
        return -1;
    }
    consume.ConsumeReduce();

    PBS2CHeroLevelUpRes res;
    res.set_herolevelupresult(result);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CHeroLevelUpRes, res);
    SavePlayer2DB(player);
    return 0;
}

int DeleteHero(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBC2SDeleteHeroReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CDeleteHeroRes res;
    player.lock()->GetHeroData().GetHeroPack().RemoveHero(req.heroindex());
    res.set_deleteheroresult(0);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CDeleteHeroRes, res);
    SavePlayer2DB(player);
    return 0;
}

int GetHeroTeam(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBS2CGetHeroTeamRes res;
    int result = player.lock()->GetHeroData().HeroTeam2PB(*res.mutable_heroteam());
    res.set_getheroteamresult(result);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetHeroTeamRes, res);
    return 0;
}

int UpdateHeroTeam(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBC2SUpdateHeroTeamReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);

    PBS2CUpdateHeroTeamRes res;
    player.lock()->GetHeroData().GetHeroTeam().PB2Team(req);
    res.set_updateheroteamresult(0);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CUpdateHeroTeamRes, res);
    SavePlayer2DB(player);
    return 0;
}

int BuyHeroLogic(int HeroID, CPlayer& player)
{
    lua_State* L = lua_open();
    luaopen_base(L);
    lua_tinker::dofile(L, "LuaCfg/BuyHero.lua");

    int ConsumeNumber = lua_tinker::get<int>(L, "BuyHeroConsume");

    lua_close(L);

    CConsume consume(ConsumeNumber, (int)ConsumeNumerical, (int)ConsumeGold, player);
    if (!consume.CheckConsume())
    {
        return -1;
    }
    CDrop drop(1, (int)DropNonCumulative, (int)DropHero, HeroID, player);
    if (!drop.CheckPack())
    {
        return -1;
    }
    consume.ConsumeReduce();
    drop.DropAdd();
    return 0;
}

int BuyHero(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    
    PBC2SBuyHeroReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    
    PBS2CBuyHeroRes res;
    int result = BuyHeroLogic(req.heroid(), *player.lock());
    res.set_buyheroresult(result);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CBuyHeroRes, res);
    SavePlayer2DB(player);
    return 0;
}

int GetPlayerList(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBS2CGetPlayerListRes res;
    res.set_getplayerlistresult(0);
    PlayerManager::GetInstance().PlayerList2PB(res);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CGetPlayerListRes, res);
    return 0;
}

int AddGold(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBC2SAddGoldReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    player.lock()->AddGold(req.goldnumber());

    PBS2CAddGoldRes res;
    res.set_addgoldresult(0);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CAddGoldRes, res);
    SavePlayer2DB(player);
    return 0;
}

int AddEmpirical(const Data& data, std::weak_ptr<CPlayer> player)
{
    if (!SendCheck(player))
    {
        return -1;
    }
    PBC2SAddEmpiricalReq req;
    req.ParseFromArray(data.buffer, data.bufferLength);
    player.lock()->AddEmpirical(req.empiricalnumber());

    PBS2CAddEmpiricalRes res;
    res.set_addempiricalresult(0);
    player.lock()->GetSocket().lock()->SendBuff((int)TypeS2CAddEmpiricalRes, res);
    SavePlayer2DB(player);
    return 0;
}



CProcess::CProcess()
{
    RegisterFunction((int)TypeC2SLoginCheckReq, LoginCheck);
    RegisterFunction((int)TypeC2SLoginRegisterReq, LoginRegister);
    RegisterFunction((int)TypeC2SCreateUserReq, CreateUser);
    RegisterFunction((int)TypeC2SDeleteUserReq, DeleteUser);
    RegisterFunction((int)TypeC2SGetPlayerInfoReq, GetPlayerInfo);
    RegisterFunction((int)TypeC2SGetHeroListReq, GetHeroList);
    RegisterFunction((int)TypeC2SGetHeroInfoReq, GetHeroInfo);
    RegisterFunction((int)TypeC2SHeroLevelUpReq, HeroLevelUp);
    RegisterFunction((int)TypeC2SDeleteHeroReq, DeleteHero);
    RegisterFunction((int)TypeC2SGetHeroTeamReq, GetHeroTeam);
    RegisterFunction((int)TypeC2SUpdateHeroTeamReq, UpdateHeroTeam);
    RegisterFunction((int)TypeC2SBuyHeroReq, BuyHero);
    RegisterFunction((int)TypeC2SGetPlayerListReq, GetPlayerList);
    RegisterFunction((int)TypeC2SAddGoldReq, AddGold);
    RegisterFunction((int)TypeC2SAddEmpiricalReq, AddEmpirical);
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

