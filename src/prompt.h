/* Everything prompts.
 */

#include <stdint.h>

#include "err.h"

#define _SIZE_PROMPT_LINE_CHARS 96

#ifndef __PROMPT_H
#define __PROMPT_H

/* Questions asked to the user.
 */
typedef char prompt_line[_SIZE_PROMPT_LINE_CHARS];
typedef struct prompt_list_t
{
    prompt_line year;
    prompt_line month;
    prompt_line day;
    prompt_line day_str;
} prompt_list_t;

/* Ask a prompt through CLI. Integer input required.
 * Tests whether input is valid.
 */
err_code_e prompt_ask_int(prompt_line *line, uint32_t *input);

/* Ask a prompt through CLI. String input required.
 * Max buffersize is PROMPT_CHAR_BUF_SIZE_CHARS chars.
 * Tests whether input is valid.
 */
err_code_e prompt_ask_char(prompt_line *line, char *buffer, uint8_t size);

#endif // __PROMPT_H