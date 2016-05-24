#ifndef _DROP_H_
#define _DROP_H_

class CPlayer;

enum eDropType
{
    DropNumerical = 0,
    DropNonCumulative
};

enum eDropGoodsType
{
    DropGold = 0,
    DropEmpirical,
    DropHero
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
