#include "../../libft/libft.h"
#include "minishell.h"

static int  is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}

static int  count_words(const char *s)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (s[i])
    {
        while (is_whitespace(s[i]))
            i++;
        if (!s[i])
            break;
        count++;
        while (s[i] && !is_whitespace(s[i]))
            i++;
    }
    return (count);
}

static void fill_words(char **out, const char *s)
{
    int i;
    int k;
    int start;

    i = 0;
    k = 0;
    while (s[i])
    {
        while (is_whitespace(s[i]))
            i++;
        if (!s[i])
            break;
        start = i;
        while (s[i] && !is_whitespace(s[i]))
            i++;
        out[k++] = ft_substr(s, start, i - start);
    }
    out[k] = NULL;
}

static char **split_whitespace(const char *s)
{
    char    **out;
    int     count;

    count = count_words(s);
    out = malloc(sizeof(char *) * (count + 1));
    if (!out)
        return (NULL);

    fill_words(out, s);
    return (out);
}

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
            parts = split_whitespace(arr[i]->str);
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
            parts = split_whitespace(arr[i]->str);
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

