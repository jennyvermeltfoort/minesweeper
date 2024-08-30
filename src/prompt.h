/*

*/

#include "err.h"

#define _SIZE_PROMPT_LINE_CHARS 96

#ifndef __PROMPT_H
#define __PROMPT_H

typedef char prompt_line[_SIZE_PROMPT_LINE_CHARS];
typedef struct prompt_list_t
{
    prompt_line year;
    prompt_line month;
    prompt_line day;
    prompt_line day_str;
} prompt_list_t;

err_code_e prompt_ask_int(prompt_line *line, int *input);
err_code_e prompt_ask_char(prompt_line *line, char *buffer, int size);

#endif // __PROMPT_H