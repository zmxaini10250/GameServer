#include "Timer.h" 

int CTimerItem::TimePass(timeval passtime)
{
    if (passtime.tv_usec < 0 || passtime.tv_sec < 0)
    {
        return 0;
    }
    return 0;
}

int CTimerItem::AddEvent(EventFunction function)
{
    FunctionList.insert(function);
    return 0;
}

int CTimerManager::TimePass(timeval passtime)
{
    return 0;
}

