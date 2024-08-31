/*
Opdracht 1, programmeermethoden.
Questionaire whether the user is compatible with an university study.

Door: Jenny Vermeltfoort.
Date initial: 8-30-2024
*/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <cstdlib>

#include "date.h"
#include "err.h"
#include "prompt.h"
#include "math.h"

/* Metadata. */
#define _STUDENT_NUMBER 3787494
#define _STUDENT_NAME "Jenny Vermeltfoort"
#define _DATE_UPDATED "8-30-2024"
#define _OPDRACHT "1"
#define _STUDIE "Bachelor Informatica"

/* Informal prompts.
 */
prompt_list_t prompt_list_t_informal = {
    .day_str = "Welke dag ben je geboren? (ma, di, wo, do, vr, za, zo)",
    .math = "Vind het antwoord van het volgende probleem: ",
    .math_1 = "Geef de teller van je antwoord.",
    .math_2 = "Geef de noemer van je antwoord.",
    .math_3 = "Geef het antwoord met twee decimalen na de comma.",
    .math_fault = "Jammer genoeg is het antwoord niet goed, we gaan verder.",
    .ok_exact = "Helemaal goed, je bent geschikt voor een exacte studie.",
    .ok_beta = "Helemaal goed, je bent geschikt voor een beta studie.",
    .done = "Je bent helemaal klaar!",
    .err_invalid = "Je gegeven informatie klopt niet helemaal, probeer het opnieuw."};

/* Formal prompts.
 */
prompt_list_t prompt_list_t_formal = {
    .year = "Verstrek je geboorte jaar.",
    .month = "Verstrek de maand waarin je geboren bent.",
    .day = "Verstrek de dag waarop je geboren bent.",
    .day_str = "Verstrek de dag waarop u bent geboren. (ma, di, wo, do, vr, za, zo)",
    .math = "Verstrek het antwoord van het volgende probleem: ",
    .math_1 = "Geef de teller van het antwoord.",
    .math_2 = "Geef de noemer van het antwoord.",
    .math_3 = "Geef het antwoord met twee decimalen na de comma.",
    .math_fault = "Helaas is dit fout, de vragenlijst gaat door.",
    .ok_exact = "Helemaal goed, u bent geschikt voor een exacte studie.",
    .ok_beta = "Helemaal goed, u bent geschikt voor een beta studie.",
    .done = "U heeft de vragenlijst afgerond, een prettige dag gewenst.",
    .err_invalid = "De gegeven informatie is niet valide, probeer het opnieuw."};

/* Print infoblock.
 */
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
    uint32_t input_int32 = 0;
    int16_t input_int16 = 0;
    float input_float = 0;
    prompt_list_t *prompt_list = &prompt_list_t_formal;
    uint8_t index = 0;

    const uint8_t input_char_size = 2;
    char input_char[input_char_size] = {0};

    time_t time_raw = time(NULL);
    struct tm *time_input = {0};
    time_input = localtime(&time_raw);

    date_day_e day_expected;
    date_day_e day_input;

    typedef struct  DATE_PROMPT_ITEM {
        prompt_line_t * line;
        date_type_e type;
    } date_prompt_item_t;

    const  uint8_t date_prompt_list_size = 3;
    date_prompt_item_t date_prompt_list[date_prompt_list_size] = {
        {.line = &prompt_list->year,
         .type = DATE_TYPE_YEAR},
        {.line = &prompt_list->month,
         .type = DATE_TYPE_MONTH},
        {.line = &prompt_list->day,
         .type = DATE_TYPE_DAY},
    };

    print_info();

    for (; index < date_prompt_list_size; index++)
    {
        rt_val = prompt_ask_uint32(date_prompt_list[index].line, &input_int32);
        if (rt_val != ERR_OK)
        {
            goto err_invalid;
        }
        
        rt_val = date_test_input(time_input, &input_int32, date_prompt_list[index].type);
        if (rt_val == ERR_INVALID_INPUT)
        {
            goto err_invalid;
        }
        else if (rt_val == ERR_AGE_UNDER_TEN)
        {
            printf("Je bent to jong om deze vragenlijst in te vullen.\n");
            return rt_val;
        }
    }

    if (date_get_age(time_input) < 30)
    {
        prompt_list = &prompt_list_t_informal;
    }

    day_expected = date_convert_to_day(time_input);
    rt_val = prompt_ask_char(&prompt_list->day_str, input_char, input_char_size);
    if (rt_val != ERR_OK)
    {
        goto err_invalid;
    }
    day_input = date_char_convert_to_day(input_char);
    if (day_input == DATE_DAY_INVALID)
    {
        prompt_print_line(&prompt_list->err_invalid);
        return rt_val;
    }

    if (day_input != day_expected)
    {
        printf("De opgegeven dag is niet valide, gegevens verwijderd!\n");
        goto end;
    }

    math_init();
    prompt_print_line(&prompt_list->math);
    printf("\t");
    math_print_problem_string();
    printf("\n");

    rt_val = prompt_ask_int16(&prompt_list->math_1, &input_int16);
    if (rt_val != ERR_OK)
    {
        prompt_print_line(&prompt_list->err_invalid);
        return rt_val;
    }
    if (!math_test_answer(&input_int16, NULL, MATH_TYPE_ANSWER_TELLER))
    {
        goto math_end_fault;
    }
    rt_val = prompt_ask_int16(&prompt_list->math_2, &input_int16);
    if (rt_val != ERR_OK)
    {
        prompt_print_line(&prompt_list->err_invalid);
        return rt_val;
    }
    if (!math_test_answer(&input_int16, NULL, MATH_TYPE_ANSWER_NOEMER))
    {
        goto math_end_fault;
    }

    rt_val = prompt_ask_float(&prompt_list->math_3, &input_float);
    if (rt_val != ERR_OK)
    {
        prompt_print_line(&prompt_list->err_invalid);
        return rt_val;
    }
    if (!math_test_answer(NULL, &input_float, MATH_TYPE_ANSWER_FLOAT))
    {
        goto math_end_fault;
    }

    prompt_print_line(&prompt_list->ok_exact);
    goto end;

math_end_fault:
    prompt_print_line(&prompt_list->math_fault);
    goto end;

err_invalid:
    prompt_print_line(&prompt_list->err_invalid);
    goto end;

end:
    prompt_print_line(&prompt_list->done);
    return rt_val;
};
