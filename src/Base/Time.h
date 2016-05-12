#ifndef _TIME_H_
#define _TIME_H_

#include "../ObjectPool/SingletonObject.hpp"

class CTime;

typedef CSingletonObject<CTime> Time;

class CTime
{
    public:
        void GetLasTickTime(struct timeval& time)const;
        int GetNowTime(struct timeval& time)const;
        int UpdateLastTickTime();
        int GetLastTimeInterval(struct timeval& time)const;
    protected:
        CTime();
        ~CTime(){};
        struct timeval LasTickTime;
};

#endif
