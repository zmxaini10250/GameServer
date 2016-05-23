#include "Hero.h"

#include <unordered_map>
#include <memory>

#include "../protobuf/ClientMessage.pb.h"
#include "../ObjectPool/ObjectPack.hpp"

int CHero::Hero2PB(PBHero& pb)const
{
    pb.set_heroinfoid(HeroInfoID);
    return 0;
}

int CHero::HeroInfo2PB(PBHeroInfo& pb)const
{
    return 0;
}

int CHeroPack::AddHero(int HeroID)
{
    return 0;
}

bool CHeroPack::isFull()
{
    return pack.isFull();
}

int CHeroPack::HeroList2PB(PBS2CGetHeroListRes& pb)const
{
    const std::unordered_map<int, std::shared_ptr<CHero>> tempack = pack.GetObjectMap();
    for (std::unordered_map<int, std::shared_ptr<CHero>>::const_iterator it = tempack.begin(); it != tempack.cend(); ++it)
    {
        it->second->Hero2PB(*pb.add_herolist());
    }
    return 0;
}
