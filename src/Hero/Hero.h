#ifndef _HERO_H_
#define _HERO_H_

#include <memory>
#include <vector>
#include "../ObjectPool/ObjectPack.hpp"
#include "../protobuf/ClientMessage.pb.h"
#include "../ObjectPool/SingletonObject.hpp"

const int HeroPackSize = 50;
const int HeroTeamSize = 6;

class CHeroAttributeManager;
typedef CSingletonObject<CHeroAttributeManager> HeroAttributeManager;

class CHeroAttribute
{
    public:
        CHeroAttribute(){}
        ~CHeroAttribute(){}
        int HeroAttribute2PB(PBHeroInfo& pb)const;
        int GetSpeed()const;
        int GetDefend()const;
        int GetAvoid()const;
        int GetAttach()const;
        int GetHealthPoint(int level)const;
    private:
        int Speed;
        int Defend;
        int Avoid;
        int Attach;
        int BaseHealthPoint;

        int HealthPointGrow;
};

class CHeroSkill
{
    public:
        CHeroSkill(){}
        virtual ~CHeroSkill(){}
    protected:
        void SkillEffect();
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
        int GetHeroAttributeID();
        int GetLevel();
        int GetHeroInfoID();
    private:
        int HeroInfoID;
        int HeroAttributeID;
        int HeroSkillID;
        int Level;
        int Empirical;
};

class CHeroPack
{
    public:
        CHeroPack(){}
        ~CHeroPack(){}
        bool isFull();
        int GetFreeSpace();
        int AddHero(int HeroID);
        int RemoveHero(int HeroIndex);
        std::weak_ptr<CHero> GetHero(int index);
        int HeroList2PB(PBS2CGetHeroListRes& pb)const;
    private:
        CObjectPack<CHero, HeroPackSize> pack;
};

class CHeroAttributeManager
{
    protected:
        CHeroAttributeManager(){};
        ~CHeroAttributeManager(){};
    public:
        std::weak_ptr<CHeroAttribute> GetHeroAttribute(int HeroAttributeID);
};

class CHeroTeam
{
    public:
        CHeroTeam(){};
        ~CHeroTeam(){};
        const std::vector<int>& GetTeam()const;
        int UpdateTeam(const std::vector<int>& newTeam);
        int PB2Team(const PBC2SUpdateHeroTeamReq& pb);
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
    private:
        CHeroPack pack;
        CHeroTeam team;
};

#endif
