/*

*/

#include <stdio.h>
#include "prompt.h"

#define PROMPT_CHAR_BUF_SIZE_CHARS 64

err_code_e
prompt_ask_int(prompt_line *line, int *input)
{
    printf("%s\n", *line);
    fflush(stdout);
    if (scanf("%d", input))
    {
        return ERR_OK;
    }

    return ERR_INVALID_INPUT;
}

err_code_e
prompt_ask_char(prompt_line *line, char *buffer, int size)
{
    char buf[PROMPT_CHAR_BUF_SIZE_CHARS] = {0};

    printf("%s\n", *line);
    fflush(stdout);

    if (scanf("%s", buf) == -1)
    {
        return ERR_INVALID_INPUT;
    }

    for (int i = 0; i < size; i++)
    {
        buffer[i] = buf[i];
    }

    return ERR_OK;
}
