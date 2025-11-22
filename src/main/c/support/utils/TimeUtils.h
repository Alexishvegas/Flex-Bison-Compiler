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
int daysInMonth(int year, int month);
int weekday(int year, int month, int day);
int normalizeWeekday(int tm_wday);
void timeToString(Time *time, char *out, size_t outlen);

#endif