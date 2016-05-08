#ifndef _TIME_H_
#define _TIME_H_

#include "../ObjectPool/SingletonObject.hpp"

class CTime;

typedef CSingletonObject<CTime> Time;

bool timevalGreater(const struct timeval& left, const struct timeval& right);
bool timevalCheck(const struct timeval& time);
const struct timeval timevalSubtract(const struct timeval& left, const struct timeval& right);
const struct timeval timevalAdd(const struct timeval& left, const struct timeval& right);

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
        int GetTime(struct timeval& time)const;
        struct timeval LasTickTime;
};

#endif
