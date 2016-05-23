#ifndef _HERO_H_
#define _HERO_H_

#include "../ObjectPool/ObjectPack.hpp"
#include "../protobuf/ClientMessage.pb.h"

const int HeroPackSize = 50;

class CHeroAttribute
{
    public:
        CHeroAttribute(){}
        ~CHeroAttribute(){}
    private:
        int Speed;
        int Defend;
        int Avoid;
        int Attach;
        int HealthPoint;

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
    private:
        int HeroInfoID;
        int HeroAttributeID;
        int HeroSkillID;

        int Empirical;

};

class CHeroPack
{
    public:
        CHeroPack(){}
        ~CHeroPack(){}
        bool isFull();
        int AddHero(int HeroID);
        int HeroList2PB(PBS2CGetHeroListRes& pb)const;
    private:
        CObjectPack<CHero, HeroPackSize> pack;
};

#endif
