#include "PlayerData.h"

CPlayer::CPlayer(int PlayerID):PlayerID(PlayerID)
{

}


///////////////////////////////////////////////////////////////////////////

std::weak_ptr<CPlayer> CPlayerManager::CreatePlayer(int PlayerID)
{
    PlayerList::const_iterator it = list.find(PlayerID);
    if (it != list.cend())
    {
        return std::weak_ptr<CPlayer>();
    }
    PlayerPool::Ptr p(PlayerPool::GetInstance().CreateObject(PlayerID));
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

