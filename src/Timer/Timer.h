#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>
#include <unordered_set>
#include <vector>
#include <unordered_map>

#include "../ObjectPool/ObjectPool.hpp"
#include "../ObjectPool/SingletonObject.hpp"

class CTimerManager;

typedef struct timeval timeval;
typedef void(*EventFunction)();
typedef CSingletonObject<CTimerManager> TimerManager;

bool timevalGreater(const timeval& left, const timeval& right);
bool timevalSubtract(timeval left, const timeval& right);
bool timevalCopy(timeval left, const timeval& right);

enum eTimeEventType
{
    DailyRefresh = 0,

    TimeEventTypeEnd
};

const int TimeEventTypeCount = (int)eTimeEventType::TimeEventTypeEnd;

class CTimerItem
{
    public:
        CTimerItem(const timeval& interval, const timeval& delaytime, int times = 1, bool loop = false):times(times), loop(loop), interval(interval), lasttime(delaytime){}
        CTimerItem(const timeval& interval, int times = 1, bool loop = false):times(times), loop(loop), interval(interval), lasttime(interval){}
        int TimePass(timeval passtime);
        virtual ~CTimerItem(){}
        int AddEvent(EventFunction function);
    protected:
        int times;
        const bool loop;
        const timeval interval;
        timeval lasttime;
        std::unordered_set<EventFunction> FunctionList;
};

class CTimerManager
{
    public:
        int TimePass(timeval passtime);

    protected:
        CTimerManager(){}
        virtual ~CTimerManager(){}

        typedef std::unordered_map<int, CTimerItem> TimerList;
        TimerList timerList;
};


#endif
