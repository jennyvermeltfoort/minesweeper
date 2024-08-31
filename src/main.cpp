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
#include "question.h"

/* Metadata. */
#define _STUDENT_NUMBER 3787494
#define _STUDENT_NAME "Jenny Vermeltfoort"
#define _DATE_UPDATED "8-30-2024"
#define _OPDRACHT "1"
#define _STUDIE "Bachelor Informatica"

typedef struct DATE_PROMPT_ITEM
{
    prompt_line_t *line;
    date_type_e type;
} date_prompt_item_t;

typedef struct MATH_PROMPT_ITEM
{
    prompt_line_t *line;
    math_type_answer_e type;
} math_prompt_item_t;

/* Informal prompts.
 */
prompt_list_t prompt_list_t_informal = {
    .day_str = "Welke dag ben je geboren? (ma, di, wo, do, vr, za, zo)",
    .math = "Vind het antwoord van het volgende probleem: ",
    .math_teller = "Geef de teller van je antwoord.",
    .math_noemer = "Geef de noemer van je antwoord.",
    .math_float = "Geef het antwoord met twee decimalen na de komma.",
    .math_fault = "Jammer genoeg is het antwoord niet goed, we gaan verder.",
    .question = "Geef hier het antwoord op de vraag. (a, b, c, d)",
    .question_fault = "Helaas klopt dat niet helemaal, waarschijnlijk ben je niet geschikt voor een opleiding aan de universiteit.",
    .ok_exact = "Helemaal goed, je bent geschikt voor een exacte studie.",
    .ok_beta = "Helemaal goed, je bent geschikt voor een beta studie.",
    .done = "Je bent helemaal klaar!",
    .err_invalid = "Je gegeven informatie klopt niet helemaal, probeer het opnieuw."};

/* Formal prompts.
 */
prompt_list_t prompt_list_formal = {
    .year = "Geef je geboorte jaar.",
    .month = "Geef de maand waarin je geboren bent.",
    .day = "Geef de dag waarop je geboren bent.",
    .day_str = "Verstrek de dag waarop u bent geboren. (ma, di, wo, do, vr, za, zo)",
    .math = "Verstrek het antwoord van het volgende probleem: ",
    .math_teller = "Geef de teller van het antwoord.",
    .math_noemer = "Geef de noemer van het antwoord.",
    .math_float = "Geef het antwoord met twee decimalen na de komma.",
    .math_fault = "Helaas is dit fout, mogelijk is een beta studie iets voor u, de vragenlijst gaat door.",
    .question = "Verstrek het antwoord op de vraag. (a, b, c, d)",
    .question_fault = "Helaas is het antwoord fout, u bent waarschijnlijk niet geschikt voor een opleiding aan de universiteit.",
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
    uint8_t index = 0;

    time_t time_raw = time(NULL);
    struct tm *time_input = {0};
    time_input = localtime(&time_raw);

    srand(time(NULL));

    prompt_list_t *prompt_list = &prompt_list_formal;
    question_item_t *question_item = &question_list_formal[rand() % QUESTION_LIST_SIZE];
    char input_char_question = 0;

    // List of prompts asked for birthday questions.
    const uint8_t date_prompt_list_size = 3;
    date_prompt_item_t date_prompt_list[date_prompt_list_size] = {
        {.line = &prompt_list->year,
         .type = DATE_TYPE_YEAR},
        {.line = &prompt_list->month,
         .type = DATE_TYPE_MONTH},
        {.line = &prompt_list->day,
         .type = DATE_TYPE_DAY},
    };
    const uint8_t input_char_size_day = 2;
    char input_char_day[input_char_size_day] = {0};
    date_day_e day_expected = DATE_DAY_INVALID;
    date_day_e day_input = DATE_DAY_INVALID;

    // List of prompts asked for math questions.
    const uint8_t math_prompt_list_size = 2;
    math_prompt_item_t math_prompt_list[math_prompt_list_size] = {
        {.line = &prompt_list->math_teller,
         .type = MATH_TYPE_ANSWER_TELLER},
        {.line = &prompt_list->math_noemer,
         .type = MATH_TYPE_ANSWER_NOEMER},
    };

    print_info();

    for (index = 0; index < date_prompt_list_size; index++)
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
            printf("De datum klopt niet of je bent to jong om deze vragenlijst in te vullen of .\n");
            return rt_val;
        }
    }

    if (date_get_age(time_input) < 30)
    {
        prompt_list = &prompt_list_t_informal;
        question_item = &question_list_informal[rand() % QUESTION_LIST_SIZE];
    }

    day_expected = date_convert_to_day(time_input);
    rt_val = prompt_ask_char(&prompt_list->day_str, input_char_day, input_char_size_day);
    if (rt_val != ERR_OK)
    {
        goto err_invalid;
    }
    day_input = date_char_convert_to_day(input_char_day);
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

    for (index = 0; index < math_prompt_list_size; index++)
    {
        rt_val = prompt_ask_int16(math_prompt_list[index].line, &input_int16);
        if (rt_val != ERR_OK)
        {
            goto err_invalid;
        }
        if (!math_test_answer(&input_int16, NULL, math_prompt_list[index].type))
        {
            goto math_end_fault;
        }
    }

    rt_val = prompt_ask_float(&prompt_list->math_float, &input_float);
    if (rt_val != ERR_OK)
    {
        goto err_invalid;
    }
    if (!math_test_answer(NULL, &input_float, MATH_TYPE_ANSWER_FLOAT))
    {
        goto math_end_fault;
    }

    prompt_print_line(&prompt_list->ok_exact);
    goto end;

math_end_fault:
    prompt_print_line(&prompt_list->math_fault);
    printf("\n");

    printf("%s\n", question_item->question);
    for (index = 0; index < QUESTION_ITEM_CHOICES_AMOUNT; index++)
    {
        printf("%s\n", question_item->choices[index]);
    }
    printf("\n");

    rt_val = prompt_ask_char(&prompt_list->question, &input_char_question, 1);
    if (rt_val != ERR_OK)
    {
        goto err_invalid;
    }

    if ((input_char_question | 0x20) != question_item->answer)
    {
        printf("Het juiste antwoord was %c.\n", question_item->answer);
        prompt_print_line(&prompt_list->question_fault);
        goto end;
    }

    prompt_print_line(&prompt_list->ok_beta);

    return rt_val;

err_invalid:
    prompt_print_line(&prompt_list->err_invalid);
    goto end;

end:
    prompt_print_line(&prompt_list->done);
    return rt_val;
};
