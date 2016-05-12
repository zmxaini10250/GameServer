#include "TimeValue.h"
#include <sys/time.h>
#include <stdio.h>

int GetTime(struct timeval& time)
{
    return gettimeofday(&time, NULL);
}


const struct timeval timeToDayEnd()
{
    struct timeval time;
    GetTime(time);
    time.tv_sec %= secondsofday;
    struct timeval daytime;
    daytime.tv_usec = 0;
    daytime.tv_sec = secondsofday;
    time = timevalSubtract(daytime, time);
    return time;
}


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
        subtime.tv_usec = usecofsec - right.tv_usec + left.tv_usec;
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
    if (time.tv_sec >= usecofsec)
    {
        time.tv_sec -= usecofsec;
        flag = true;
    }
    time.tv_sec = left.tv_sec + right.tv_sec;
    if (flag)
    {
        ++time.tv_sec;
    }
    return time;
}

