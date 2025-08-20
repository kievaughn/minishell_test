#include "../libft/libft.h"
#include "minishell.h"

char **split_expanded_tokens(char **arr)
{
    int i;
    int j;
    int k;
    int total;
    char **parts;
    char **out;

    i = 0;
    total = 0;
    while (arr && arr[i])
    {
        if (ft_strchr(arr[i], '\'') || ft_strchr(arr[i], '"'))
            total++;
        else
        {
            parts = ft_split(arr[i], ' ');
            if (!parts)
                return (free_cmd(arr), NULL);
            total += count_strings(parts);
            free_cmd(parts);
        }
        i++;
    }
    out = malloc(sizeof(char *) * (total + 1));
    if (!out)
        return (free_cmd(arr), NULL);
    i = 0;
    k = 0;
    while (arr && arr[i])
    {
        if (ft_strchr(arr[i], '\'') || ft_strchr(arr[i], '"'))
            out[k++] = arr[i];
        else
        {
            parts = ft_split(arr[i], ' ');
            free(arr[i]);
            if (!parts)
            {
                free_cmd(out);
                free_cmd(arr + i + 1);
                free(arr);
                return (NULL);
            }
            j = 0;
            while (parts[j])
                out[k++] = parts[j++];
            free(parts);
        }
        i++;
    }
    out[k] = NULL;
    free(arr);
    return (out);
}
