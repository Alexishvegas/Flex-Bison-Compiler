#ifndef TIME_UTILS_HEADER
#define TIME_UTILS_HEADER

#include <stdlib.h>
#include <time.h>

typedef struct Time Time;

struct Time {
    int hour;
    int minute;
};

int compareTime(Time *t1, Time *t2);
int weekday(int year, int month, int day);
int normalizeWeekday(int tm_wday);

#endif