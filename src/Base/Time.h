#ifndef _TIME_H_
#define _TIME_H_

#include "../ObjectPool/SingletonObject.hpp"

class CTime;

typedef CSingletonObject<CTime> Time;

class CTime
{
    protected:
        CTime(){}
        ~CTime(){}
        const struct timeval GetLasTickTime();
};

#endif
