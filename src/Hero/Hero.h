#ifndef _HERO_H_
#define _HERO_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include "../ObjectPool/ObjectPack.hpp"
#include "../protobuf/ClientMessage.pb.h"
#include "../ObjectPool/SingletonObject.hpp"
#include "../ObjectPool/ObjectPool.hpp"
const int HeroPackSize = 50;
const int HeroTeamSize = 6;

class CHeroAttributeManager;
class CHeroAttribute;

typedef CObjectPool<CHeroAttribute> HeroAttributePool;
typedef CSingletonObject<CHeroAttributeManager> HeroAttributeManager;

class CHeroAttribute
{
    public:
        CHeroAttribute(){}
        ~CHeroAttribute(){}
        int HeroAttribute2PB(PBHeroInfo& pb)const;
        int GetSpeed()const;
        int SetSpeed(int Number);
        int GetDefend()const;
        int SetDefend(int Number);
        int GetAvoid()const;
        int SetAvoid(int Number);
        int GetAttach()const;
        int SetAttach(int Number);
        int SetBaseHealthPoint(int Number);
        int SetHealthPointGrow(int Number);
        int GetHealthPoint(int level)const;
    private:
        int Speed;
        int Defend;
        int Avoid;
        int Attach;
        int BaseHealthPoint;

        int HealthPointGrow;
};

class CHero
{
    public:
        CHero(){}
        ~CHero(){}
        int Hero2PB(PBHero& pb)const;
        int HeroInfo2PB(PBHeroInfo& pb)const;
        int GetUpLevelEmpirical();
        int LevelUp();
        int GetHeroID()const{ return HeroID; }
        int GetHeroAttributeID();
        int GetLevel()const;
        int GetEmpirical()const;
        int GetHeroInfoID();
        int SetLevel(int Number);
        int SetEmpircal(int Number);
        int SetHeroID(int Number);
    private:
        int HeroID;
        int Level;
        int Empirical;
};

class CHeroPack
{
    public:
        CHeroPack(){}
        ~CHeroPack(){}
        void Clear();
        bool isFull();
        int GetFreeSpace();
        int AddHero(int HeroID);
        int RemoveHero(int HeroIndex);
        std::weak_ptr<CHero> GetHero(int index);
        int HeroList2PB(PBS2CGetHeroListRes& pb)const;
        const std::unordered_map<int, std::shared_ptr<CHero>>& GetHeroMap()const;
    private:
        CObjectPack<CHero, HeroPackSize> pack;
};

class CHeroAttributeManager
{
    protected:
        CHeroAttributeManager(){};
        ~CHeroAttributeManager(){};
        typedef std::unordered_map<int, HeroAttributePool::Ptr> HeroAttributeList;
        HeroAttributeList list;
    public:
        std::weak_ptr<CHeroAttribute> GetHeroAttribute(int HeroAttributeID);
};

class CHeroTeam
{
    public:
        CHeroTeam(){};
        ~CHeroTeam(){};
        void Clear();
        const std::vector<int>& GetTeam()const;
        int UpdateTeam(const std::vector<int>& newTeam);
        int PB2Team(const PBC2SUpdateHeroTeamReq& pb);
        bool InTeam(int index);
    private:
        std::vector<int> team;       
};

class CHeroData
{
    public:
        CHeroData(){}
        ~CHeroData(){}
        CHeroPack& GetHeroPack();
        CHeroTeam& GetHeroTeam();
        int HeroTeam2PB(PBHeroTeam& pb);
        int DB2Hero(const DBHeroInfo &db);
        int Hero2DB(DBHeroInfo &db);
    private:
        CHeroPack pack;
        CHeroTeam team;
};

#endif
