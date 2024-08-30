/*


Date initial: 8-30-2024
*/

#include <stdio.h>
#include <time.h>
#include "date.h"
#include "err.h"
#include "prompt.h"

#define _STUDENT_NUMBER 3787494
#define _STUDENT_NAME "Jenny Vermeltfoort"
#define _DATE_UPDATED "8-30-2024"

prompt_list_t prompt_list_t_informal = {
    .day_str = "Welke dag ben je geboren? (ma, di, wo, do, vr, za, zo)",
};
prompt_list_t prompt_list_t_formal = {
    .year = "Verstrek uw geboorte jaar.",
    .month = "Verstrek de maand waarin u geboren bent.",
    .day = "Verstrek de dag waarop u geboren bent.",
    .day_str = "Verstrek de dag waarop u bent geboren. (ma, di, wo, do, vr, za, zo)",
};

void print_info(void)
{
    printf("Naam: %s\n", _STUDENT_NAME);
    printf("Student nummer: %i\n", _STUDENT_NUMBER);
    printf("Laatst geupdate: %s\n", _DATE_UPDATED);
}

int main(int argc, char **argv)
{
    err_code_e rt_val = ERR_OK;
    int input_int = 0;

    int input_char_size = 2;
    char input_char[input_char_size] = {0};

    time_t time_raw = time(NULL);
    struct tm *time_input = {0};
    time_input = localtime(&time_raw);

    date_day_e day_expected;
    date_day_e day_input;

    print_info();

    rt_val = prompt_ask_int(&prompt_list_t_formal.year, &input_int);
    if (rt_val != ERR_OK)
    {
        printf("%s", "De gegeven informatie is niet valide, probeer het opnieuw.\n");
        return rt_val;
    }
    rt_val = date_test_input(time_input, &input_int, date_TYPE_YEAR);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }

    rt_val = prompt_ask_int(&prompt_list_t_formal.month, &input_int);
    if (rt_val != ERR_OK)
    {
        printf("%s", "De gegeven informatie is niet valide, probeer het opnieuw.\n");
        return rt_val;
    }
    rt_val = date_test_input(time_input, &input_int, date_TYPE_MONTH);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }

    rt_val = prompt_ask_int(&prompt_list_t_formal.day, &input_int);
    if (rt_val != ERR_OK)
    {
        printf("%s", "De gegeven informatie is niet valide, probeer het opnieuw.\n");
        return rt_val;
    }
    rt_val = date_test_input(time_input, &input_int, date_TYPE_DAY);
    if (rt_val != ERR_OK)
    {
        return rt_val;
    }

    day_expected = date_convert_to_day(time_input);
    rt_val = prompt_ask_char(&prompt_list_t_formal.day_str, input_char, input_char_size);
    if (rt_val != ERR_OK)
    {
        printf("%s", "De gegeven informatie is niet valide, probeer het opnieuw.\n");
        return rt_val;
    }
    day_input = date_char_convert_to_day(input_char);
    if (day_input == DATE_DAY_INVALID)
    {
        printf("%s", "De gegeven informatie is niet valide, probeer het opnieuw.\n");
        return rt_val;
    }

    if (day_input != day_expected)
    {
        printf("Invalide dag opgegeven, gegevens verwijderd!\n");
    }

    return 0;
};
