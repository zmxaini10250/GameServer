#ifndef _HERO_H_
#define _HERO_H_

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
        int HeroInfoIndex;
        int Hero;
};

#endif
