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
#define _OPDRACHT "1"
#define _STUDIE "Bachelor Informatica"

prompt_list_t prompt_list_t_informal = {
    .day_str = "Welke dag ben je geboren? (ma, di, wo, do, vr, za, zo)",
};
prompt_list_t prompt_list_t_formal = {
    .year = "Verstrek je geboorte jaar.",
    .month = "Verstrek de maand waarin je geboren bent.",
    .day = "Verstrek de dag waarop je geboren bent.",
    .day_str = "Verstrek de dag waarop u bent geboren. (ma, di, wo, do, vr, za, zo)",
};

void print_info(void)
{
    printf("Naam: %s\n", _STUDENT_NAME);
    printf("Student nummer: %i\n", _STUDENT_NUMBER);
    printf("Laatst geupdate: %s\n", _DATE_UPDATED);
    printf("Opdracht: %s\n", _OPDRACHT);
    printf("Studie: %s\n", _STUDIE);
    printf("\n");
    printf("Vragenlijst: Ben jij geschikt voor een studie aan de universiteit?\n");
}

int main(int argc, char **argv)
{
    err_code_e rt_val = ERR_OK;
    int input_int = 0;
    prompt_list_t *prompt_list = &prompt_list_t_formal;

    int input_char_size = 2;
    char input_char[input_char_size] = {0};

    time_t time_raw = time(NULL);
    struct tm *time_input = {0};
    time_input = localtime(&time_raw);

    date_day_e day_expected;
    date_day_e day_input;

    print_info();

    rt_val = prompt_ask_int(&prompt_list->year, &input_int);
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

    rt_val = prompt_ask_int(&prompt_list->month, &input_int);
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

    rt_val = prompt_ask_int(&prompt_list->day, &input_int);
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

    if (date_get_age(time_input) < 30)
    {
        prompt_list = &prompt_list_t_informal;
    }

    day_expected = date_convert_to_day(time_input);
    rt_val = prompt_ask_char(&prompt_list->day_str, input_char, input_char_size);
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
        printf("De opgegeven dag is niet valide, gegevens verwijderd!\n");
    }

    return 0;
};
