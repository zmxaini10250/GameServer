#ifndef _HERO_H_
#define _HERO_H_

#include "../ObjectPool/ObjectPack.hpp"

const int HeroPackSize = 50;

class CHeroInfo
{
    public:
        CHeroInfo(){}
        ~CHeroInfo(){}
    private:

};

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
    private:
        int HeroInfoID;
        int HeroAttributeID;
        int HeroSkillID;

        int Empirical;

};

class CHeroPack
{
    public:
        CHeroPack()
        ~CHeroPack(){}
        bool isFull();
        int AddHero(int HeroID);
    private:
        CObjectPack<CHero, HeroPackSize> pack;
};

#endif
