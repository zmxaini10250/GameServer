#include <sys/time.h>
#include <time.h>
#include <cstdio>
#include <typeinfo>
#include <iostream>
#include <limits>
//const time_t usecofsec = 1000;

const time_t secondsofminute = 60;
const time_t secondsofhour = 60 * 60;
//const time_t secondsofday = 24 * 60 * 60;

const time_t minutesofhour = 60;
const time_t hourofday = 24;


//const time_t dayofweek = 7;
//const time_t beginweekday = 4;

void testtime();


int main()
{
    return 0;
}

void testtime()
{
    time_t tNow = time(0);
    struct tm ptm;
    localtime_r(&tNow, &ptm);

    printf("second\n");
    printf("%d\n",ptm.tm_sec);

    printf("mintes\n");
    printf("%d\n",ptm.tm_min);

    printf("hour\n");
    printf("%d\n",ptm.tm_hour);

    printf("day\n");
    printf("%d\n",ptm.tm_mday);

    printf("month\n");
    printf("%d\n",ptm.tm_mon);

    printf("year\n");
    printf("%d\n",ptm.tm_year);


    printf("weak day\n");
    printf("%d\n",ptm.tm_wday);


    printf("year day\n");
    printf("%d\n",ptm.tm_yday);

    printf("\n\n");
    
    struct timeval timev;
    gettimeofday(&timev, 0);

    printf("second\n");
    printf("%ld\n",timev.tv_sec%secondsofminute);

    printf("mintes\n");
    printf("%ld\n",(timev.tv_sec/secondsofminute)%minutesofhour);
    
    printf("hour\n");
    printf("%ld\n",(timev.tv_sec/secondsofhour)%hourofday);
    
    printf("all second\n");
    printf("%ld\n",timev.tv_sec);
    
    printf("usecond\n");
    printf("%ld\n",timev.tv_usec);
}
