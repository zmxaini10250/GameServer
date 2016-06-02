#include "Drop.h"

#include "../Player/PlayerData.h"

int NumericalDropAdd(int Number, int DropGoodsType, CPlayer& player)
{
    switch(DropGoodsType)
    {
        case (int)DropGold:
            player.AddGold(Number);
            break;
        case (int)DropEmpirical:
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
        case (int)DropHero:
            return player.GetHeroData().GetHeroPack().GetFreeSpace() >= Number;
        default:
            return false;
    }
    return false;
}

int NonCumulativeDropAdd(int Number, int DropGoodsType, int ID, CPlayer& player)
{
    switch(DropGoodsType)
    {
        case (int)DropHero:
            for (int i = 0; i < Number; ++i)
            {
                player.GetHeroData().GetHeroPack().AddHero(ID);
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
        case (int)DropNumerical:
            CheckResult = true;
            break;
        case (int)DropNonCumulative:
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
            case (int)DropNumerical:
                return NumericalDropAdd(Number, DropGoodsType, player);
            case (int)DropNonCumulative:
                return NonCumulativeDropAdd(Number, DropGoodsType, ID, player);
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
