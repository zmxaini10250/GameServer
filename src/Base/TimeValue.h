#ifndef _TIMEVALUE_H_
#define _TIMEVALUE_H_

#include <time.h>
#include <sys/time.h>
const time_t usecofsec = 1000000;

const time_t secondsofminute = 60;
const time_t secondsofhour = 60 * 60;
const time_t secondsofday = 24 * 60 * 60;

const time_t minutesofhour = 60;
const time_t hourofday = 24;


const time_t dayofweek = 7;
const time_t beginweekday = 4;

const struct timeval timeToDayEnd();
int GetTime(struct timeval& time);


bool timevalGreater(const struct timeval& left, const struct timeval& right);
bool timevalCheck(const struct timeval& time);
const struct timeval timevalSubtract(const struct timeval& left, const struct timeval& right);
const struct timeval timevalAdd(const struct timeval& left, const struct timeval& right);


#endif
