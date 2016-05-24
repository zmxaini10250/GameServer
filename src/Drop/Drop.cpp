#include "Drop.h"

#include "../Player/PlayerData.h"

int NumericalDropAdd(int Number, int DropGoodsType, CPlayer& player)
{
    switch(DropGoodsType)
    {
        case (int)Gold:
            player.AddGold(Number);
            break;
        case (int)Empirical:
            player.AddEmpirical(Number);
            break;
        default:
            return -1;
    }
    return 0;
}

bool NonCumulativeCheckPack(int Number, int DropGoodsType, CPlayer& player)
{
    switch(DropGoodsType)
    {
        case (int)Hero:
            return player.GetHeroData().GetHeroPack().GetFreeSpace() >= Number;
        default:
            return false;
    }
    return false;
}

int NonCumulativeDropAdd(int Number, int DropGoodsType, CPlayer& player)
{
    switch(DropGoodsType)
    {
        case (int)Hero:
            for (int i = 0; i < Number; ++i)
            {
                player.GetHeroData().GetHeroPack().AddHero(1);
            }
        default:
            return -1;
    }
    return 0;
}

bool CDrop::CheckPack()
{
    AlreadyCheck = true;
    switch(DropType)
    {
        case (int)Numerical:
            CheckResult = true;
            break;
        case (int)NonCumulative:
            CheckResult = NonCumulativeCheckPack(Number, DropGoodsType, player);
            break;
        default:
            return false;
    }
    return CheckResult;
}

int CDrop::DropAdd()
{
    if (AlreadyCheck && CheckResult)
    {
        switch(DropType)
        {
            case (int)Numerical:
                return NumericalDropAdd(Number, DropGoodsType, player);
            case (int)NonCumulative:
                return NonCumulativeDropAdd(Number, DropGoodsType, player);
            default:
                return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}
