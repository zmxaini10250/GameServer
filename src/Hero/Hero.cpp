#include "Hero.h"

#include <unordered_map>
#include <memory>

#include "../protobuf/ClientMessage.pb.h"
#include "../ObjectPool/ObjectPack.hpp"
#include "../ObjectPool/SingletonObject.hpp"

int CHero::Hero2PB(PBHero& pb)const
{
    pb.set_heroinfoid(HeroInfoID);
    return 0;
}

int CHero::HeroInfo2PB(PBHeroInfo& pb)const
{
    Hero2PB(*pb.mutable_hero());
    std::weak_ptr<CHeroAttribute> p = HeroAttributeManager::GetInstance().GetHeroAttribute(HeroAttributeID);
    if (p.expired())
    {
        return -1;
    }
    CHeroAttribute& attribute = *p.lock();
    attribute.HeroAttribute2PB(pb);
    pb.set_level(Level);
    return 0;
}

int CHeroPack::AddHero(int HeroID)
{
    return 0;
}

bool CHeroPack::isFull()
{
    return pack.isFull();
    //return true;
}

int CHeroPack::HeroList2PB(PBS2CGetHeroListRes& pb)const
{
    const std::unordered_map<int, std::shared_ptr<CHero>>& tempack = pack.GetObjectMap();
    for (std::unordered_map<int, std::shared_ptr<CHero>>::const_iterator it = tempack.begin(); it != tempack.cend(); ++it)
    {
        PBHero* hero = pb.add_herolist();
        hero->set_heroindex(it->first);
        it->second->Hero2PB(*hero);
    }
    
    return 0;
}

int CHeroPack::GetFreeSpace()
{
    return pack.GetFreeSpace();
}

int CHero::GetUpLevelEmpirical()
{
    return Level * 100;
}

int CHero::LevelUp()
{
    Empirical += GetUpLevelEmpirical();
    ++Level;
    return 0;
}

int CHero::GetHeroAttributeID()
{
    return HeroAttributeID;
}

int CHero::GetLevel()
{
    return Level;
}

int CHero::GetHeroInfoID()
{
    return HeroInfoID;
}

std::weak_ptr<CHero> CHeroPack::GetHero(int index)
{
    return pack.GetObject(index);
}

int CHeroPack::RemoveHero(int HeroIndex)
{
    return pack.RemoveObject(HeroIndex);
}

int CHeroAttribute::HeroAttribute2PB(PBHeroInfo& pb)const
{
    pb.set_speed(Speed);
    pb.set_defend(Defend);
    pb.set_avoid(Avoid);
    pb.set_attach(Attach);
}

int CHeroAttribute::GetSpeed()const
{
    return Speed;
}

int CHeroAttribute::GetDefend()const
{
    return Defend;
}

int CHeroAttribute::GetAvoid()const
{
    return Avoid;
}

int CHeroAttribute::GetAttach()const
{
    return Attach;
}

int CHeroAttribute::GetHealthPoint(int level)const
{
    return level * 1000 * HealthPointGrow + BaseHealthPoint;
}

std::weak_ptr<CHeroAttribute> CHeroAttributeManager::GetHeroAttribute(int HeroAttributeID)
{
    return std::weak_ptr<CHeroAttribute>();
}

const std::vector<int>& CHeroTeam::GetTeam()const
{
    return team;
}

int CHeroTeam::UpdateTeam(const std::vector<int>& newTeam)
{
    team.clear();
    int i = 0;
    for (std::vector<int>::const_iterator it = newTeam.begin(); it != newTeam.cend() && i < HeroTeamSize; ++it, ++i)
    {
        if (*it >= 0 && *it < HeroPackSize)
        {
            team.push_back(*it);
        }
        else
        {
            return -1;
        }
        return 0;
    }
}

int CHeroTeam::PB2Team(const PBC2SUpdateHeroTeamReq& pb)
{
    team.clear();
    int size = pb.heroindex_size();
    for (int i = 0; i < size && i < HeroTeamSize; ++i)
    {
        int index = pb.heroindex(i);
        if (index >= 0 && index < HeroPackSize)
        {
            team.push_back(index);
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

CHeroPack& CHeroData::GetHeroPack()
{
    return pack;
}

CHeroTeam& CHeroData::GetHeroTeam()
{
    return team;
}

int CHeroData::HeroTeam2PB(PBHeroTeam& pb)
{
    int HealthPoint = 0;
    int Speed = 0;
    int Defend = 0; 
    int Avoid = 0;
    int Attach = 0; 

    const std::vector<int>& teamVector = team.GetTeam();

    int i = 0;
    for (std::vector<int>::const_iterator it = teamVector.cbegin(); it != teamVector.cend() && i < HeroTeamSize; ++it, ++i)
    {
        if (*it >= 0 && *it < HeroPackSize)
        {
            std::weak_ptr<CHero> hero = pack.GetHero(*it);
            if (hero.expired())
            {
                return -1;
            }
            std::weak_ptr<CHeroAttribute> attribute = HeroAttributeManager::GetInstance().GetHeroAttribute(hero.lock()->GetHeroAttributeID());
            if (attribute.expired())
            {
                return -1;
            }
            std::shared_ptr<CHeroAttribute> attributeptr = attribute.lock();
            HealthPoint += attributeptr->GetHealthPoint(hero.lock()->GetLevel());
            Speed += attributeptr->GetSpeed();
            Defend += attributeptr->GetDefend();
            Avoid += attributeptr->GetAvoid();
            Attach += attributeptr->GetAttach();
            pb.add_heroinfoid(hero.lock()->GetHeroInfoID());
        }
        else
        {
            return -1;
        }
        pb.set_healthpoint(HealthPoint);
        pb.set_speed(Speed);
        pb.set_defend(Defend);
        pb.set_avoid(Avoid);
        pb.set_attach(Attach);
        return 0;
    }
}
