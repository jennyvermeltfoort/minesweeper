/*

*/

#include <stdio.h>
#include "prompt.h"

err_code
prompt_ask_int(prompt_line *line, int *input)
{
    printf("%s\n", *line);
    fflush(stdout);
    if (scanf("%d", input))
    {
        return ERR_OK;
    }

    printf("%s", "De gegeven informatie niet valide, probeer het opnieuw.\n");
    return ERR_INVALID_INPUT;
}
