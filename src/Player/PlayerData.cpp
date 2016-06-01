#include "PlayerData.h"

#include <memory>
#include <string>

#include "../Hero/Hero.h"
#include "../TCPServer/TCPClientSocket.h"
#include "../ObjectPool/SingletonObject.hpp"
#include "../ObjectPool/ObjectPool.hpp"

std::weak_ptr<CTCPClientSocket> CPlayer::GetSocket()
{
    return socketPtr;
}

void CPlayer::SetSocket(const std::weak_ptr<CTCPClientSocket>& Socket)
{
    socketPtr = Socket;
}

int CPlayer::GetPlayerID()
{
    return PlayerID;
}

int CPlayer::PlayerInfo2PB(PBPlayerInfo &PlayerInfo)
{
    if (PlayerID <= 0)
    {
        return -1;
    }
    PlayerInfo.set_playerid(PlayerID);
    PlayerInfo.set_username(Username);
    PlayerInfo.set_gold(Gold);
    PlayerInfo.set_empirical(Empirical);   
    return 0;
}

std::string CPlayer::GetUsername()
{
    return Username;
}

CHeroData& CPlayer::GetHeroData()
{
    return HeroData;
}

int CPlayer::GetGold()
{
    return Gold;
}

int CPlayer::ConsumeGold(int ConsumeNum)
{
    Gold -= ConsumeNum;
    return 0;
}

int CPlayer::AddGold(int AddNum)
{
    Gold += AddNum;
    return 0;
}

int CPlayer::GetEmpirical()
{
    return Empirical;
}


int CPlayer::ConsumeEmpirical(int ConsumeNum)
{
    Empirical -= ConsumeNum;
    return 0;
}


int CPlayer::AddEmpirical(int AddNum)
{
    Empirical += AddNum;
    return 0;
}

int CPlayer::Player2DB(DBPlayer &db)
{
    db.set_playerid(PlayerID);
    db.set_playername(Username);
    db.set_gold(Gold);
    db.set_empirical(Empirical);
    HeroData.Hero2DB(*(db.mutable_hero()));
    return 0;
}
int CPlayer::DB2Player(const DBPlayer &db)
{
    PlayerID = db.playerid();
    Username = db.playername();
    Gold = db.gold();
    Empirical = db.empirical();
    if (db.has_hero())
    {
        HeroData.DB2Hero(db.hero());
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////

std::weak_ptr<CPlayer> CPlayerManager::CreatePlayer(int PlayerID,const std::weak_ptr<CTCPClientSocket>& socketPtr)
{
    PlayerList::const_iterator it = list.find(PlayerID);
    if (it != list.cend())
    {
        return std::weak_ptr<CPlayer>();
    }
    PlayerPool::Ptr p(PlayerPool::GetInstance().CreateObject(PlayerID, socketPtr));
    if (p != nullptr)
    {
        list.insert(std::make_pair(PlayerID, p));
    }
    return std::weak_ptr<CPlayer>(p);
}

std::weak_ptr<CPlayer> CPlayerManager::GetPlayer(int PlayerID)
{
    PlayerList::const_iterator it = list.find(PlayerID);
    if (it == list.cend())
    {
        return std::weak_ptr<CPlayer>();
    }
    return std::weak_ptr<CPlayer>(it->second);
}

int CPlayerManager::PlayerList2PB(PBS2CGetPlayerListRes& res)
{
    for (PlayerList::const_iterator it = list.begin(); it != list.cend(); ++it)
    {
        PBPlayer *player = res.add_playerlist();
        player->set_playerid(it->second->GetPlayerID());
        player->set_username(it->second->GetUsername().c_str());
    }
    return 0;
}
