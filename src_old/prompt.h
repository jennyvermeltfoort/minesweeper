/* Everything prompts.
 */

#include <stdint.h>

#include "err.h"

#define _SIZE_PROMPT_LINE_CHARS 128

#ifndef __PROMPT_H
#define __PROMPT_H

/* Questions asked to the user.
 */
typedef char prompt_line_t[_SIZE_PROMPT_LINE_CHARS];
typedef struct prompt_list_t
{
    prompt_line_t year;
    prompt_line_t month;
    prompt_line_t day;
    prompt_line_t day_str;
    prompt_line_t math;
    prompt_line_t math_teller;
    prompt_line_t math_noemer;
    prompt_line_t math_float;
    prompt_line_t math_fault;
    prompt_line_t question;
    prompt_line_t question_fault;
    prompt_line_t ok_exact;
    prompt_line_t ok_beta;
    prompt_line_t done;
    prompt_line_t err_invalid;
} prompt_list_t;

/* Print prompt_line_t in CLI.
 */
void prompt_print_line(prompt_line_t *line);

/* Ask a prompt through CLI. Integer input required.
 * Tests whether input is valid.
 */
err_code_e prompt_ask_uint32(prompt_line_t *line, uint32_t *input);

/* Ask a prompt through CLI. Integer input required.
 * Tests whether input is valid.
 */
err_code_e prompt_ask_int16(prompt_line_t *line, int16_t *input);

/* Ask a prompt through CLI. String input required.
 * Max buffersize is PROMPT_CHAR_BUF_SIZE_CHARS chars.
 * Tests whether input is valid.
 */
err_code_e prompt_ask_char(prompt_line_t *line, char *buffer, uint8_t size);

/* Ask a prompt through CLI. String input required.
 * Max buffersize is PROMPT_CHAR_BUF_SIZE_CHARS chars.
 * Tests whether input is valid.
 */
err_code_e prompt_ask_float(prompt_line_t *line,  float *input);

#endif // __PROMPT_H