#ifndef NEO_TIME_H
#define NEO_TIME_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char* current_time()
{
    static char time_str[100]; // Persists after function exits
    time_t t = time(NULL);
    struct tm date = *localtime(&t);

    snprintf(time_str, sizeof(time_str), "%d-%02d-%02d\t %02d:%02d:%02d",
             date.tm_year + 1900, date.tm_mon + 1, date.tm_mday,
             date.tm_hour, date.tm_min, date.tm_sec);

    return time_str; // Safe return of a static string
}
#endif
