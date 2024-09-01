/* Everything date's.
 */

#include <time.h>
#include <stdint.h>
#include "err.h"

#ifndef __date_H
#define __date_H

/* Type of date.
 * Function: date_test_input.
 */
typedef enum DATE_TYPE
{
    DATE_TYPE_YEAR,
    DATE_TYPE_MONTH,
    DATE_TYPE_DAY,
} date_type_e;

/* All days in a week.
 */
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

/* Test whether time_input data is underaged (10) or has valid timedata.
 * Return ERR_AGE_UNDER_TEN if age is under ten, else ERR_OK.
 * Note: manipulates time_input, month is supstracted by 1, conversion fix.
 */
err_code_e date_test_input(struct tm *time_input, uint32_t *input, date_type_e type);

/* Convert time_input to correlating date_day_e value.
 * Returns the date_day_e.
 */
date_day_e date_convert_to_day(struct tm *time_input);

/* Convert input (ma,di,wo,do,vr,za,zo) to correlating date_day_e value.
 * Returns the date_day_e.
 */
date_day_e date_char_convert_to_day(char input[2]);

/* Retrieve the age from now till time_input data.
 * Returns the age.
 */
uint8_t date_get_age(struct tm *time_input);

#endif // __date_H
