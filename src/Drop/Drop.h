#ifndef _DROP_H_
#define _DROP_H_

class CPlayer;

enum eDropType
{
    Numerical = 0,
    NonCumulative
};

enum eDropGoodsType
{
    Gold = 0,
    Empirical,
    Hero
};

class CDrop
{
    public:
        CDrop(int Number, int DropType, int DropGoodsType, CPlayer& player):Number(Number), DropType(DropType), DropGoodsType(DropGoodsType), player(player){}
        ~CDrop(){}
        bool CheckPack();
        int DropAdd();
    private:
        bool AlreadyCheck = false;
        bool CheckResult = false;
        int Number;
        int DropType;
        int DropGoodsType;
        CPlayer& player;
};

#endif
