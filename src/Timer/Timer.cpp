#include "Timer.h" 

#include <unordered_set>
#include <unordered_map>

#include "../Base/Time.h"
#include "../Base/TimeValue.h"

int CTimerItem::TimePass(const struct timeval& passtime)
{
    if (!timevalCheck(passtime))
    {
        return 0;
    }
    struct timeval timenow, subtime;
    Time::GetInstance().GetNowTime(timenow);
    subtime = timevalSubtract(timenow, lasttime);
    if (timevalGreater(subtime, interval))
    {
        lasttime = timevalAdd(lasttime, interval);
        for (FunctionList::const_iterator it = functionList.begin(); it != functionList.cend(); ++it)
        {
            (*it)();
        }
    }
    return 0;
}

int CTimerItem::AddEvent(EventFunction function)
{
    if (function != NULL)
    {
        functionList.insert(function);
    }
    return 0;
}

int CTimerManager::TimePass(const struct timeval& passtime)
{
    for (TimerList::iterator it = timerList.begin(); it != timerList.cend(); ++it)
    {
        it->second.TimePass(passtime);
    }
    return 0;
}

CTimerManager::CTimerManager()
{
    timerList.insert(std::make_pair<eTimeEventType, CTimerItem>(DailyRefresh, CTimerItem({secondsofday, 0}, timeToDayEnd(), true)));
    timerList.insert(std::make_pair<eTimeEventType, CTimerItem>(Second5Refresh, CTimerItem({5, 0}, timeToDayEnd(), true)));
    timerList.insert(std::make_pair<eTimeEventType, CTimerItem>(MillSecond500Refresh, CTimerItem({0, 500000}, timeToDayEnd(), true)));
}

int CTimerManager::RegisterEvent(eTimeEventType type, EventFunction event)
{
    TimerList::iterator it = timerList.find((int)type);
    if (it != timerList.cend())
    {
        it->second.AddEvent(event);
    }
    return 0;
}
