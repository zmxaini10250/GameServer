#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>
#include "../ObjectPool/ObjectPool.hpp"
#include "../ObjectPool/SingletonObject.hpp"

typedef struct timeval timeval;

bool timevalGreater(const timeval& left, const timeval& right);
bool timevalSubtract(timeval left, const timeval& right);
bool timevalCopy(timeval left, const timeval& right);


class CTimerItem
{
    public:
        CTimerItem(timeval interval, timeval delaytime, int times = 1):times(times), interval(interval), lasttime(delaytime){}
        CTimerItem(timeval interval, int times = 1):times(times), interval(interval){}
        void TimePass(timeval passtime);
        virtual ~CTimerItem(){}
    protected:
        int times;
        const timeval interval;
        timeval lasttime;
};

class CTimerManager
{
    public:
    protected:
};


#endif
