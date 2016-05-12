#include "Time.h"

#include <sys/time.h>

#include "TimeValue.h"

CTime::CTime()
{
    GetTime(LasTickTime);
}

void CTime::GetLasTickTime(struct timeval& time)const
{
    time = LasTickTime;
}

int CTime::GetNowTime(struct timeval& time)const
{
    return GetTime(time);
}

int CTime::UpdateLastTickTime()
{
    return GetTime(LasTickTime);
}

int CTime::GetLastTimeInterval(struct timeval& time)const
{
    struct timeval timenow;
    int error = GetTime(timenow);
    if (error != 0)
    {
        return error;
    }
    time = timevalSubtract(timenow, LasTickTime);
    if (!timevalCheck(time))
    {
        return -1;
    }
    return 0;
}


