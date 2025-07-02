#include "minishell.h"
#include "../libft/libft.h"

static int part_count(char *tok)
{
    int i = 0;
    int start = 0;
    int count = 0;

    while (tok[i])
    {
        if (tok[i] == '>' || tok[i] == '<')
        {
            if (i - start > 0)
                count++;
            if (tok[i] == '>' && tok[i + 1] == '>')
            {
                count++;
                i += 2;
            }
            else
            {
                count++;
                i++;
            }
            start = i;
        }
        else
            i++;
    }
    if (i - start > 0)
        count++;
    return (count);
}

static int total_parts(char **arr)
{
    int total = 0;
    int i = 0;
    while (arr && arr[i])
    {
        total += part_count(arr[i]);
        i++;
    }
    return (total);
}

char    **split_redirs(char **arr)
{
    char    **out;
    int     i;
    int     j;
    int     start;
    int     idx;
    char    quote;

    out = malloc(sizeof(char *) * (total_parts(arr) + 1));
    if (!out)
        return (free_cmd(arr), NULL);
    idx = 0;
    i = 0;
    while (arr[i])
    {
        j = 0;
        start = 0;
        quote = 0;
        while (arr[i][j])
        {
            if (!quote && (arr[i][j] == '\'' || arr[i][j] == '"'))
                quote = arr[i][j];
            else if (quote && arr[i][j] == quote)
                quote = 0;

            if (!quote && (arr[i][j] == '>' || arr[i][j] == '<'))
            {
                if (j - start > 0)
                    out[idx++] = ft_substr(arr[i], start, j - start);
                if (arr[i][j] == '>' && arr[i][j + 1] == '>')
                {
                    out[idx++] = ft_strdup(">>");
                    j += 2;
                }
                else
                {
                    char    op[2];
                    op[0] = arr[i][j];
                    op[1] = '\0';
                    out[idx++] = ft_strdup(op);
                    j++;
                }
                start = j;
            }
            else
                j++;
        }
        if (j - start > 0)
            out[idx++] = ft_substr(arr[i], start, j - start);
        free(arr[i]);
        i++;
    }
    out[idx] = NULL;
    free(arr);
    return (out);
}