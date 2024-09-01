/* Everything math.
 */

#include <stdint.h>

#ifndef __MATH_H
#define __MATH_H

typedef enum MATH_TYPE_ANSWER {
MATH_TYPE_ANSWER_TELLER,
MATH_TYPE_ANSWER_NOEMER,
MATH_TYPE_ANSWER_FLOAT,

} math_type_answer_e;

/* Initialize math module, when called all values are (re)generated.
* Requires preinitialized srand.
*/
void math_init();

/* Test whether the input answer is correct.
* Returns true when correct, false when not.
*/
bool math_test_answer(int16_t* answer_int, float *answer_float, math_type_answer_e type);

/* Prints the math problem string using printf.
*/
void math_print_problem_string(void);

#endif // __MATH_H


