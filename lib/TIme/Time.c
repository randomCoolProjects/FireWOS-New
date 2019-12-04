#include "Time.h"
#include <inttypes.h>
#include <stdbool.h>
#include <code_tools.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

char* DaysOfWeek[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednsday",
    "Thursday",
    "Friday",
    "Saturday"
};

char* Month[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
};

int DaysOfMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

Time_t* mainTime;

void timeUpdate (Time_t *t)
{
    int micros = esp_timer_get_time();
    t->ms = micros * MILLIS - t->_real_ms * 1000;

    while(t->ms >= 1000)
    {
        t->_real_ms++;
        t->second++;
        t->ms-=1000;
    }
    while(t->second >= 60)
    {
        t->second -= 60;
        t->minute++;
    }
    while(t->minute>=60)
    {
        t->hour++;
        t->minute-=60;
    }
    while(t->hour>=24)
    {
        t->day++;
        t->hour-=24;
    }

    int daysmonth = timeGetDaysOfMonth(t);
    while(t->day > daysmonth)
    {
        t->month++;
        t->day -= daysmonth;
    }

    while(t->month > 12)
    {
        t->year++;
        t->month -= 12;
    }

}

void time_task(void)
{
    Time_t time = NEW_TIME;
    mainTime = malloc(sizeof(Time_t));
    while (1)
    {
        timeUpdate(&time);
        *mainTime = time;
    }
}

char* timeGetDayOfWeek(Time_t* t)
{
    int dayOfWeek = t->day;
    while(dayOfWeek > 7) dayOfWeek-= 7;
    return DaysOfWeek[dayOfWeek-1];
}

char* timeGetMonth(Time_t* t)
{
    return Month[t->day-1];
}

int timeGetDaysOfMonth(Time_t* t)
{
    return DaysOfMonth[t->month-1];
}