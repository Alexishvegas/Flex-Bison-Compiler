#include "TimeUtils.h"

int compareTime(Time *t1, Time *t2) {
    if (t1 == NULL || t2 == NULL) {
        // Podrías tratar esto como error, pero para evitar crash:
        return 0;
    }

    if (t1->hour < t2->hour) return -1;
    if (t1->hour > t2->hour) return 1;

    // horas iguales, comparar minutos
    if (t1->minute < t2->minute) return -1;
    if (t1->minute > t2->minute) return 1;

    return 0; // son iguales
}

int weekday(int year, int month, int day) {
    struct tm t = {0};
    t.tm_year = year - 1900;  // años desde 1900
    t.tm_mon = month - 1;     // 0 = enero
    t.tm_mday = day;

    mktime(&t);               // normaliza y calcula tm_wday

    return t.tm_wday;         // 0 = Sunday ... 6 = Saturday
}

int normalizeWeekday(int tm_wday) {
    // convierte 0=Sunday → 6, y 1=Monday → 0, etc.
    return (tm_wday + 6) % 7;
}

static int daysInMonth(int year, int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            // Leap year: divisible by 4, but centuries divisible by 400
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                return 29;
            else
                return 28;
    }
    return 0;
}
