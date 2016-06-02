#include "Hero.h"

#include <unordered_map>
#include <memory>
#include <vector>
#include "../protobuf/ClientMessage.pb.h"
#include "../ObjectPool/ObjectPack.hpp"
#include "../ObjectPool/SingletonObject.hpp"
#include "../DBServer/DBServer.h"
int CHero::Hero2PB(PBHero& pb)const
{
    pb.set_heroinfoid(HeroID);
    return 0;
}

int CHero::HeroInfo2PB(PBHeroInfo& pb)const
{
    Hero2PB(*pb.mutable_hero());
    std::weak_ptr<CHeroAttribute> p = HeroAttributeManager::GetInstance().GetHeroAttribute(HeroID);
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
    std::weak_ptr<CHeroAttribute> attribute = HeroAttributeManager::GetInstance().GetHeroAttribute(HeroID);
    if (attribute.expired())
    {
        return -1;
    }
    int index = pack.CreateObjectIndex();
    if (pack.GetObject(index).expired())
    {
        return -1;
    }
    pack.GetObject(index).lock()->SetHeroID(HeroID);
    return index;
}

bool CHeroPack::isFull()
{
    return pack.isFull();
    //return true;
}

void CHeroPack::Clear()
{
    pack.Clear();
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

const std::unordered_map<int, std::shared_ptr<CHero>>& CHeroPack::GetHeroMap()const
{
    return pack.GetObjectMap();
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

int CHero::SetLevel(int Number)
{
    if (Number > 0)
    {
        Level = Number;
    }
}
int CHero::SetEmpircal(int Number)
{
    if (Number > 0)
    {
        Empirical = Number;
    }
}

int CHero::SetHeroID(int Number)
{
    HeroID = Number;
}

int CHero::GetHeroAttributeID()
{
    return HeroID;
}

int CHero::GetLevel()const
{
    return Level;
}
int CHero::GetEmpirical()const
{
    return Empirical;
}

int CHero::GetHeroInfoID()
{
    return HeroID;
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

int CHeroAttribute::SetSpeed(int Number)
{
    Speed = Number;
    return 0;
}
int CHeroAttribute::SetDefend(int Number)
{
    Defend = Number;
    return 0;
}

int CHeroAttribute::SetAvoid(int Number)
{
    Avoid = Number;
    return 0;
}

int CHeroAttribute::SetAttach(int Number)
{
    Attach = Number;
    return 0;
}

int CHeroAttribute::SetBaseHealthPoint(int Number)
{
    BaseHealthPoint = Number;
    return 0;
}

int CHeroAttribute::SetHealthPointGrow(int Number)
{
    HealthPointGrow = Number;
    return 0;
}

std::weak_ptr<CHeroAttribute> CHeroAttributeManager::GetHeroAttribute(int HeroAttributeID)
{
    HeroAttributeList::const_iterator it = list.find(HeroAttributeID);
    if (it != list.cend())
    {
        return std::weak_ptr<CHeroAttribute>(it->second);
    }
    HeroAttributePool::Ptr p(HeroAttributePool::GetInstance().CreateObject()); 
    if (p != nullptr)
    {
        if (DBServer::GetInstance().GetHeroData(HeroAttributeID, *p) == 0)
        {
            list.insert(std::make_pair(HeroAttributeID, p));
            return std::weak_ptr<CHeroAttribute>(p);
        }
        else
        {
            return std::weak_ptr<CHeroAttribute>();
        }
    }  
    return std::weak_ptr<CHeroAttribute>();
}

const std::vector<int>& CHeroTeam::GetTeam()const
{
    int size = team.size();
    return team;
}

void CHeroTeam::Clear()
{
    team.clear();
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
    }
    int size = team.size();
    return 0;
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
bool CHeroTeam::InTeam(int index)
{
    for (std::vector<int>::const_iterator it = team.begin(); it != team.cend(); ++it)
    {
        if (*it == index)
        {
            return true;
        }
    }
    return false;
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
    int size = teamVector.size();
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

    }
    pb.set_healthpoint(HealthPoint);
    pb.set_speed(Speed);
    pb.set_defend(Defend);
    pb.set_avoid(Avoid);
    pb.set_attach(Attach);
    return 0;
}

int CHeroData::DB2Hero(const DBHeroInfo &db)
{
    pack.Clear();
    team.Clear();
    std::vector<int> teamindex;
    for (int i = 0; i < db.heropack_size(); ++i)
    {
        int index = pack.AddHero(db.heropack(i).heroid());
        if (index == -1)
        {
            continue;
        }
        if (!pack.GetHero(index).expired())
        {
            pack.GetHero(index).lock()->SetEmpircal(db.heropack(i).empirical());
            pack.GetHero(index).lock()->SetLevel(db.heropack(i).level());
        }
        if (db.heropack(i).inteam())
        {
            teamindex.push_back(index);
        }
    }
    team.UpdateTeam(teamindex);
    return 0;
}

int CHeroData::Hero2DB(DBHeroInfo &db)
{
    const std::unordered_map<int, std::shared_ptr<CHero>>& HeroMap = pack.GetHeroMap();
    for (std::unordered_map<int, std::shared_ptr<CHero>>::const_iterator it = HeroMap.cbegin(); it != HeroMap.cend(); ++it)
    {
        DBHero *newhero = db.add_heropack();
        newhero->set_level(it->second->GetLevel());
        newhero->set_empirical(it->second->GetEmpirical());
        newhero->set_heroid(it->second->GetHeroID());
        newhero->set_inteam(team.InTeam(it->first));
    }
    return 0;
}
