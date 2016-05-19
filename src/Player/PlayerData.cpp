#include "PlayerData.h"

#include <memory>
#include <string>

#include "../TCPServer/TCPClientSocket.h"

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
    
    return 0;
}

std::string CPlayer::GetUsername()
{
    return Username;
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

