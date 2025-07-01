#include "minishell.h"

int count_strings(char **arr)
{
    int i;

    i = 0;
    while (arr && arr[i])
        i++;
    return (i);
}

