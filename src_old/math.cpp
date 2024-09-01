
#include <stdio.h>
#include <cstdlib>
#include <time.h>

#include "math.h"

#define MATH_NUMBER_LIST_SIZE 4
#define MATH_ANSWER_FLOAT_LIMIT 0.1

int16_t math_number_list[MATH_NUMBER_LIST_SIZE] = {0}; // teller_1, noemer_1, teller_2, noemer_2
int16_t math_answer_list[2] = {0};                     // teller, noemer
float math_answer_float = 0;
char math_sign;

void math_init()
{
    uint8_t i = 0;

    math_sign = (rand() % 2) ? '+' : '-';

    for (; i < MATH_NUMBER_LIST_SIZE; i++)
    {
        math_number_list[i] = rand() % 20 + 1;
    }

    if (math_number_list[2] == math_number_list[3])
    {
        math_answer_list[0] = math_number_list[0] + (math_number_list[2] * ((math_sign == '-') ? -1 : 1));
        math_answer_list[1] = math_number_list[2];
    }
    else
    {
        math_answer_list[0] = math_number_list[0] * math_number_list[3] + (math_number_list[2] * math_number_list[1] * ((math_sign == '-') ? -1 : 1));
        math_answer_list[1] = math_number_list[3] * math_number_list[1];
    }

    math_answer_float = (float)math_answer_list[0] / (float)math_answer_list[1];

    printf("%i\n", math_answer_list[0]);
    printf("%i\n", math_answer_list[1]);
    printf("%.2f\n", math_answer_float);
}

bool math_test_answer(int16_t *answer_int, float *answer_float, math_type_answer_e type)
{
    switch (type)
    {
    case MATH_TYPE_ANSWER_TELLER:
        return (answer_int != NULL && *answer_int == math_answer_list[0]);
    case MATH_TYPE_ANSWER_NOEMER:
        return (answer_int != NULL && *answer_int == math_answer_list[1]);
    case MATH_TYPE_ANSWER_FLOAT:
        return (answer_float != NULL && (math_answer_float - MATH_ANSWER_FLOAT_LIMIT) < *answer_float && *answer_float < (math_answer_float + MATH_ANSWER_FLOAT_LIMIT));
    default:
        return 0;
    }
}

void math_print_problem_string(void)
{
    printf("%i/%i %c %i/%i", math_number_list[0], math_number_list[1], math_sign, math_number_list[2], math_number_list[3]);
}