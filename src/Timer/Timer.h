#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>
#include <unordered_set>
#include <unordered_map>

#include "../ObjectPool/ObjectPool.hpp"
#include "../ObjectPool/SingletonObject.hpp"

class CTimerManager;

typedef void(*EventFunction)();
typedef CSingletonObject<CTimerManager> TimerManager;

enum eTimeEventType
{
    DailyRefresh = 0,
    Second5Refresh,
    MillSecond500Refresh,
    TimeEventTypeEnd
};

const int TimeEventTypeCount = (int)eTimeEventType::TimeEventTypeEnd;

class CTimerItem
{
    public:
        CTimerItem(const struct timeval& interval, const struct timeval& delaytime, int times = 1, bool loop = false):times(times), loop(loop), interval(interval), lasttime(delaytime){}
        CTimerItem(const struct timeval& interval, int times = 1, bool loop = false):times(times), loop(loop), interval(interval), lasttime(interval){}
        int TimePass(const struct timeval& passtime);
        virtual ~CTimerItem(){}
        int AddEvent(EventFunction function);
    protected:
        int times;
        const bool loop;
        const struct timeval interval;
        struct timeval lasttime;
        typedef std::unordered_set<EventFunction> FunctionList;
        FunctionList functionList;
};

class CTimerManager
{
    public:
        int TimePass(const struct timeval& passtime);
        
    protected:
        CTimerManager();
        virtual ~CTimerManager(){}
        int RegisterEvent(eTimeEventType type, EventFunction event);
        typedef std::unordered_map<int, CTimerItem> TimerList;
        TimerList timerList;
};


#endif
