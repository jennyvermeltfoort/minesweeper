/*

*/

#include <time.h>
#include "err.h"

#ifndef __date_H
#define __date_H

typedef enum date_TYPE
{
    date_TYPE_YEAR,
    date_TYPE_MONTH,
    date_TYPE_DAY,
} date_type_e;

typedef enum DAY
{
    DATE_DAY_MONDAY,
    DATE_DAY_TUESDAY,
    DATE_DAY_WEDNESDAY,
    DATE_DAY_THURSDAY,
    DATE_DAY_FRIDAY,
    DATE_DAY_SATERDAY,
    DATE_DAY_SUNDAY,
    DATE_DAY_INVALID,
} date_day_e;

err_code_e date_test_input(struct tm *time_input, int *input, date_type_e type);
date_day_e date_convert_to_day(struct tm *time_input);
date_day_e date_char_convert_to_day(char input[2]);

#endif // __date_H
