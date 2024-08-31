/* Prompt implementation.
 */

#include <stdio.h>
#include "prompt.h"

#define PROMPT_CHAR_BUF_SIZE_CHARS 64

void prompt_print_line(prompt_line_t *line)
{
    printf("%s\n", *line);
}

err_code_e
prompt_ask_uint32(prompt_line_t *line, uint32_t *input)
{
    prompt_print_line(line);
    fflush(stdout);
    if (scanf("%d", input))
    {
        return ERR_OK;
    }

    return ERR_INVALID_INPUT;
}

err_code_e
prompt_ask_int16(prompt_line_t *line, int16_t *input)
{
    prompt_print_line(line);
    fflush(stdout);
    if (scanf("%hd", input))
    {
        return ERR_OK;
    }

    return ERR_INVALID_INPUT;
}

err_code_e
prompt_ask_float(prompt_line_t *line, float *input)
{
    prompt_print_line(line);
    fflush(stdout);
    if (scanf("%f", input))
    {
        return ERR_OK;
    }

    return ERR_INVALID_INPUT;
}

err_code_e
prompt_ask_char(prompt_line_t *line, char *buffer, uint8_t size)
{
    char buf[PROMPT_CHAR_BUF_SIZE_CHARS] = {0};

    prompt_print_line(line);
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
