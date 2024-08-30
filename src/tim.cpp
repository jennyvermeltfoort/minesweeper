/*

*/

#include <stdio.h>

#include "tim.h"

err_code tim_update(struct tm *time_input, int *year, int *month, int *day)
{
    if (year)
    {
        time_input->tm_year = *year - 1900 + 10;
    };

    if (month)
    {
        if (!(1 <= *month && *month <= 12))
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

err_code
tim_test_age(struct tm *time_input)
{
    // Return ERR_AGE_UNDER_TEN if age is under ten, else ERR_OK.
    // Prints error message.
    time_t time_now = time(NULL);
    time_t time_converted = mktime(time_input);

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

err_code tim_test_input(struct tm *time_input, int *input, tim_type type)
{
    err_code rt_val = ERR_OK;

    switch (type)
    {
    case TIM_TYPE_YEAR:
        rt_val = tim_update(time_input, input, NULL, NULL);
        break;
    case TIM_TYPE_MONTH:
        rt_val = tim_update(time_input, NULL, input, NULL);
        break;
    case TIM_TYPE_DAY:
        rt_val = tim_update(time_input, NULL, NULL, input);
        break;
    }

    if (rt_val == ERR_INVALID_INPUT)
    {
        printf("%s", "De gegeven informatie niet valide, probeer het opnieuw.\n");
        return rt_val;
    }

    rt_val = tim_test_age(time_input);
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