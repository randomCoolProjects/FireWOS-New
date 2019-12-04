#ifndef TIME_H
#define TIME_H

#include <inttypes.h>

#define MICROS 1
#define MILLIS 0.001
#define SECOND 0.000001

// char* DaysOfWeek[];

// char* Month[];

// int DaysOfMonth[];

typedef struct
{
    int64_t _real_ms;
    int64_t ms;
    int second;
    int minute;
    int hour;
    int day;
    int month;
    int year;
} Time_t;

#define NEW_TIME {._real_ms = 0, .second = 0, .minute = 0, .hour = 0, .day = 1, .month = 1, .year = 2019}

Time_t* mainTime;

void time_task(void);

char* timeGetDayOfWeek(Time_t* t);

char* timeGetMonth(Time_t* t);

void timePrintToBuffer(Time_t* t, char* buf);

int timeGetDaysOfMonth(Time_t* t);

void timeUpdate (Time_t *t);

#endif