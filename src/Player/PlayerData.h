#ifndef _PLAYERDATA_H_
#define _PLAYERDATA_H_

#include <memory>
#include <unordered_map>

#include "../ObjectPool/ObjectPool.hpp"
#include "../ObjectPool/SingletonObject.hpp"


class CPlayer;
class CPlayerManager; 

typedef CObjectPool<CPlayer> PlayerPool;
typedef CSingletonObject<CPlayer> PlayerManager;


class CPlayer
{
    public:
        CPlayer(int PlayerID);
        ~CPlayer(){}
    private:
        int PlayerID;
        int PlayerGuild;
};

class CPlayerManager
{
    public:
        std::weak_ptr<CPlayer> CreatePlayer(int PlayerID);
        std::weak_ptr<CPlayer> GetPlayer(int PlayerID);
    protected:
        typedef std::unordered_map<int, PlayerPool::Ptr> PlayerList;
        PlayerList list;
        CPlayerManager(){}
        ~CPlayerManager(){}
};

#endif
