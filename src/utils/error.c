#include "minishell.h"
#include <stdio.h>

void    error_msg(const char *prefix, const char *msg)
{
    if (prefix && msg)
        fprintf(stderr, "%s: %s\n", prefix, msg);
    else if (msg)
        fprintf(stderr, "%s\n", msg);
}

void    error_perror(const char *context)
{
    if (context)
        perror(context);
    else
        perror("minishell");
}
