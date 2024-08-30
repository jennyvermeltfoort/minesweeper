/*

*/

#include <time.h>
#include "err.h"

#ifndef __TIM_H
#define __TIM_H

typedef enum TIM_TYPE
{
    TIM_TYPE_YEAR,
    TIM_TYPE_MONTH,
    TIM_TYPE_DAY,
} tim_type;

err_code tim_test_input(struct tm *time_input, int *input, tim_type type);

#endif // __TIM_H
