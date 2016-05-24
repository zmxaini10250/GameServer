#include "Consume.h"
#include "../Player/PlayerData.h"

bool NumericalCheckConsume(int Number, int ConsumeGoodsType, CPlayer& player)
{
    switch(ConsumeGoodsType)
    {
        case (int)ConsumeGold:
            return player.GetGold() >= Number;
        case (int)ConsumeEmpirical:
            return player.GetEmpirical() >= Number;
        default:
            return false;
    }
    return false;
}

int NumericalConsumeReduce(int Number, int ConsumeGoodsType, CPlayer& player)
{
    switch(ConsumeGoodsType)
    {
        case (int)ConsumeGold:
            player.ConsumeGold(Number);
            break;
        case (int)ConsumeEmpirical:
            player.ConsumeEmpirical(Number);
            break;
        default:
            return -1;
    }
    return 0;
}

bool CConsume::CheckConsume()
{
    AlreadyCheck = true;
    switch(ConsumeType)
    {
        case (int)ConsumeNumerical:
            CheckResult = NumericalCheckConsume(Number, ConsumeGoodsType, player);
            break;
        default:
            return false;
    }
    return CheckResult;
}

int CConsume::ConsumeReduce()
{
    if (AlreadyCheck && CheckResult)
    {
        switch(ConsumeType)
        {
            case (int)ConsumeNumerical:
                return NumericalConsumeReduce(Number, ConsumeGoodsType, player);
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
