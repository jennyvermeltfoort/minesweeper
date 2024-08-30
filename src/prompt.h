/*

*/

#include "err.h"

#define _SIZE_PROMPT_LINE_CHARS 64

#ifndef __PROMPT_H
#define __PROMPT_H

typedef char prompt_line[_SIZE_PROMPT_LINE_CHARS];
typedef struct prompt_list
{
    prompt_line year;
    prompt_line month;
    prompt_line day;
} prompt_list;



err_code prompt_ask_int(prompt_line *line, int *input);

#endif // __PROMPT_H