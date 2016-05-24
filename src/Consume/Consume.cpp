#include "Consume.h"
#include "../Player/PlayerData.h"

bool NumericalCheckConsume(int Number, int ConsumeGoodsType, CPlayer& player)
{
    switch(ConsumeGoodsType)
    {
        case (int)Gold:
            return player.GetGold() >= Number;
        case (int)Empirical:
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
        case (int)Gold:
            player.ConsumeGold(Number);
            break;
        case (int)Empirical:
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
        case (int)Numerical:
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
            case (int)Numerical:
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
