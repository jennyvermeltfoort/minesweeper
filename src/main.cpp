/*


Date initial: 8-30-2024
*/

#include <stdio.h>
#include <time.h>
#include "tim.h"
#include "err.h"
#include "prompt.h"

#define _STUDENT_NUMBER 3787494
#define _STUDENT_NAME "Jenny Vermeltfoort"
#define _DATE_UPDATED "8-30-2024"

prompt_list prompt_list_informal = {};
prompt_list prompt_list_formal = {
    .year = "Verstrek uw geboorte jaar.",
    .month = "Verstrek de maand waarin u geboren bent.",
    .day = "Verstrek de dag waarop u geboren bent.",
};

void print_info(void)
{
    printf("Naam: %s\n", _STUDENT_NAME);
    printf("Student nummer: %i\n", _STUDENT_NUMBER);
    printf("Laatst geupdate: %s\n", _DATE_UPDATED);
}

int main(int argc, char **argv)
{
    print_info();
    err_code rt_val = ERR_OK;
    int input = 0;

    time_t time_raw = time(NULL);
    struct tm *time_input = {0};
    time_input = localtime(&time_raw);

    rt_val = prompt_ask_int(&prompt_list_formal.year, &input);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }
    rt_val = tim_test_input(time_input, &input, TIM_TYPE_YEAR);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }

    rt_val = prompt_ask_int(&prompt_list_formal.month, &input);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }
    rt_val = tim_test_input(time_input, &input, TIM_TYPE_MONTH);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }

    rt_val = prompt_ask_int(&prompt_list_formal.year, &input);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }
    rt_val = tim_test_input(time_input, &input, TIM_TYPE_DAY);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }

    return 0;
};
