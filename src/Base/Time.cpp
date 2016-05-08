#include "Time.h"

#include <sys/time.h>

bool timevalGreater(const struct timeval& left, const struct timeval& right)
{
    if (left.tv_sec > right.tv_sec)
    {
        return true;
    }
    else
    {
        if ((left.tv_sec == right.tv_sec) && (left.tv_usec > right.tv_usec))
        {
            return true;
        }
    }
    return false;
}

bool timevalCheck(const struct timeval& time)
{
    if(time.tv_sec < 0 || time.tv_usec < 0)
    {
        return false;
    }
    return true;
}

const struct timeval timevalSubtract(const struct timeval& left, const struct timeval& right)
{

    struct timeval subtime;
    bool flag = false;
    if(left.tv_usec < right.tv_usec)
    {
        flag = true;
        subtime.tv_usec = 1000000 - right.tv_usec + left.tv_usec;
    }
    else
    {
        subtime.tv_usec = left.tv_usec - right.tv_usec;
    }
    subtime.tv_sec = left.tv_sec - right.tv_sec;
    if (flag)
    {
        --subtime.tv_sec;
    }
    return subtime;
}


const struct timeval timevalAdd(const struct timeval& left, const struct timeval& right)
{
    struct timeval time;
    bool flag = false;
    time.tv_usec = left.tv_usec + right.tv_usec;
    if (time.tv_sec >= 1000000)
    {
        time.tv_sec -= 1000000;
        flag = true;
    }
    time.tv_sec = left.tv_sec + right.tv_sec;
    if (flag)
    {
        ++time.tv_sec;
    }
    return time;
}


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

int CTime::GetTime(struct timeval& time)const
{
    return gettimeofday(&time, NULL);
}
