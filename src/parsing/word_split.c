#include "../libft/libft.h"
#include "minishell.h"

t_token **split_expanded_tokens(t_token **arr)
{
    int i, j, k, total;
    char **parts;
    t_token **out;

    i = 0;
    total = 0;
    while (arr && arr[i])
    {
        if (arr[i]->quoted)
            total++;
        else
        {
            parts = ft_split(arr[i]->str, ' ');
            if (!parts)
                return (free_tokens(arr), NULL);
            total += count_strings(parts);
            free_cmd(parts);
        }
        i++;
    }

    out = malloc(sizeof(t_token *) * (total + 1));
    if (!out)
        return (free_tokens(arr), NULL);

    i = 0;
    k = 0;
    while (arr && arr[i])
    {
        if (arr[i]->quoted)
        {
            out[k++] = arr[i];
        }
        else
        {
            parts = ft_split(arr[i]->str, ' ');
            free(arr[i]->str);
            free(arr[i]);
            if (!parts)
            {
                free_tokens(out);
                free_tokens(arr + i + 1);
                free(arr);
                return (NULL);
            }
            j = 0;
            while (parts[j])
                out[k++] = new_token(parts[j++], 0, 0);
            free(parts);
        }
        i++;
    }
    out[k] = NULL;

    free(arr);
    return (out);
}

