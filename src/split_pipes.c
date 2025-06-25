#include "minishell.h"
#include "libft/libft.h"

static size_t count_segments(const char *line)
{
    size_t i = 0;
    size_t count = 1;
    char quote = 0;

    while (line && line[i])
    {
        if (!quote && (line[i] == '\'' || line[i] == '"'))
            quote = line[i];
        else if (quote && line[i] == quote)
            quote = 0;
        else if (!quote && line[i] == '|')
            count++;
        i++;
    }
    return count;
}

char **split_pipes(const char *line)
{
    size_t i = 0;
    size_t start = 0;
    size_t seg = 0;
    char quote = 0;
    size_t segments = count_segments(line);
    char **arr = malloc(sizeof(char *) * (segments + 1));

    if (!arr)
        return NULL;
    while (line && line[i])
    {
        if (!quote && (line[i] == '\'' || line[i] == '"'))
            quote = line[i];
        else if (quote && line[i] == quote)
            quote = 0;
        else if (!quote && line[i] == '|')
        {
            arr[seg++] = ft_substr(line, start, i - start);
            start = i + 1;
        }
        i++;
    }
    arr[seg++] = ft_substr(line, start, i - start);
    arr[seg] = NULL;
    return arr;
}
