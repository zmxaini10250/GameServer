#include <sys/time.h>
#include <time.h>
#include <cstdio>

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

    printf("%s\n", "second");
    printf("%d\n",ptm.tm_sec);

    printf("%s\n", "mintes");
    printf("%d\n",ptm.tm_min);

    printf("%s\n", "hour");
    printf("%d\n",ptm.tm_hour);

    printf("%s\n", "day");
    printf("%d\n",ptm.tm_mday);

    printf("%s\n", "month");
    printf("%d\n",ptm.tm_mon);

    printf("%s\n", "year");
    printf("%d\n",ptm.tm_year);


    printf("%s\n", "weak day");
    printf("%d\n",ptm.tm_wday);


    printf("%s\n", "year day");
    printf("%d\n",ptm.tm_yday);

    printf("\n");
    
    struct timeval timev;
    gettimeofday(&timev, 0);

    printf("%s\n", "second");
    printf("%ld\n",timev.tv_sec%secondsofminute);

    printf("%s\n", "mintes");
    printf("%ld\n",(timev.tv_sec/secondsofminute)%minutesofhour);
    
    printf("%s\n", "hour");
    printf("%ld\n",(timev.tv_sec/secondsofhour)%hourofday);
    
    printf("%s\n", "all second");
    printf("%ld\n",timev.tv_sec);
    
    printf("%s\n", "usecond");
    printf("%ld\n",timev.tv_usec);
}
