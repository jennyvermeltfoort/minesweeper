/*

*/

#include <stdio.h>

#include "date.h"

#define _DATE_NORM_YEAR 1876 // Calculations will be normalized to zaterdag, Januari 1th 1876.
                             // Oldest person alive.
                             // 1876 is a schrikkeljaar
#define _DATE_NORM_MON 1
#define _DATE_NORM_DAY 1
#define _DAYS_IN_YEAR 365

date_day_e day_list[] = {
    DATE_DAY_SATERDAY,
    DATE_DAY_SUNDAY,
    DATE_DAY_MONDAY,
    DATE_DAY_TUESDAY,
    DATE_DAY_WEDNESDAY,
    DATE_DAY_THURSDAY,
    DATE_DAY_FRIDAY,
};

int day_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

err_code_e date_update(struct tm *time_input, int *year, int *month, int *day)
{
    if (year)
    {
        time_input->tm_year = *year;
    };

    if (month)
    {
        if (1 < *month && *month > 12)
        {
            return ERR_INVALID_INPUT;
        }
        time_input->tm_mon = *month - 1;
    };

    if (day)
    {
        if (!(1 <= *day && *day <= 31))
        {
            return ERR_INVALID_INPUT;
        }
        time_input->tm_mday = *day;
    }

    return ERR_OK;
}

err_code_e
date_test_age(struct tm *time_input)
{
    // Return ERR_AGE_UNDER_TEN if age is under ten, else ERR_OK.
    // Prints error message.
    time_t time_now = time(NULL);
    time_input->tm_year = time_input->tm_year - 1900 + 10; // idk some weird conversion in time module
    time_t time_converted = mktime(time_input);
    time_input->tm_year = time_input->tm_year + 1900 - 10;

    if (time_converted == -1)
    {
        return ERR_INVALID_INPUT;
    }

    if (difftime(time_converted, time_now) > 0)
    {
        return ERR_AGE_UNDER_TEN;
    }

    return ERR_OK;
}

err_code_e date_test_input(struct tm *time_input, int *input, date_type_e type)
{
    err_code_e rt_val = ERR_OK;

    switch (type)
    {
    case date_TYPE_YEAR:
        rt_val = date_update(time_input, input, NULL, NULL);
        break;
    case date_TYPE_MONTH:
        rt_val = date_update(time_input, NULL, input, NULL);
        break;
    case date_TYPE_DAY:
        rt_val = date_update(time_input, NULL, NULL, input);
        break;
    }

    if (rt_val == ERR_INVALID_INPUT)
    {
        printf("%s", "De gegeven informatie niet valide, probeer het opnieuw.\n");
        return rt_val;
    }

    rt_val = date_test_age(time_input);
    if (rt_val == ERR_INVALID_INPUT)
    {
        printf("%s", "De gegeven informatie niet valide, probeer het opnieuw.\n");
        return rt_val;
    }
    else if (rt_val == ERR_AGE_UNDER_TEN)
    {
        printf("%s", "Je bent to jong om deze vragenlijst in te vullen.\n");
        return rt_val;
    }
    return rt_val;
}

int date_get_age(struct tm *time_input)
{
    time_t time_now = time(NULL);
    time_input->tm_year -= 1900; // idk some weird conversion in time module
    time_t time_converted = mktime(time_input);
    time_input->tm_year += 1900;

    double sec = difftime(time_now, time_converted);
    return int(sec / 31556925.9936);
}

date_day_e date_convert_to_day(struct tm *time_input)
{
    // Converted back to seperated integers as per the assignment.
    int year = time_input->tm_year;
    int month = time_input->tm_mon;
    int day = time_input->tm_mday;

    int num_leap_days = int((year - _DATE_NORM_YEAR) / 4);
    if ((year / 4 - int(year / 4)) == 0 && (month < 2 || (month == 2 && day < 29))) // year is leap year
    {
        num_leap_days--;
    }

    int num_days_from_year = (year - _DATE_NORM_YEAR) * _DAYS_IN_YEAR;
    int num_days_from_mon = 0;
    for (int i = 0; i < month - 1; i++)
    {
        num_days_from_mon += day_month[i];
    }
    int total_days = num_days_from_year + num_days_from_mon + day + num_leap_days;

    return day_list[total_days % 7];
}

date_day_e date_char_convert_to_day(char input[2])
{
    switch (input[0])
    {
    case 'm':
        return DATE_DAY_MONDAY;
    case 'd':
        if (input[1] == 'i')
        {
            return DATE_DAY_TUESDAY;
        }
        return DATE_DAY_THURSDAY;
    case 'w':
        return DATE_DAY_WEDNESDAY;
    case 'v':
        return DATE_DAY_FRIDAY;
    case 'z':
        if (input[1] == 'a')
        {
            return DATE_DAY_SATERDAY;
        }
        return DATE_DAY_SUNDAY;
    default:
        return DATE_DAY_INVALID;
    }
}