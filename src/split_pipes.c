#include "minishell.h"
#include "libft/libft.h"

static size_t count_segments(const char *line)
{
    size_t i;
    size_t count;
    char quote;

    i = 0;
    count = 1;
    quote = 0;

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
    return (count);
}

static void add_segment(char **arr, size_t *seg, size_t *start, size_t end, const char *line)
{
    arr[*seg] = ft_substr(line, *start, end - *start);
    *seg += 1;
    *start = end + 1;
}

static void fill_segments(char **arr, const char *line)
{
    size_t i;
    size_t seg;
    size_t start;
    char quote;
    i = 0;
    seg = 0;
    start = 0;
    quote = 0;
    while (line && line[i])
    {
        if (!quote && (line[i] == '\'' || line[i] == '"'))
            quote = line[i];
        else if (quote && line[i] == quote)
            quote = 0;
        else if (!quote && line[i] == '|')
            add_segment(arr, &seg, &start, i, line);
        i++;
    }
    arr[seg++] = ft_substr(line, start, i - start);
    arr[seg] = NULL;
}

char **split_pipes(const char *line)
{
    size_t segments;
    char **arr;

    segments = count_segments(line);
    arr = malloc(sizeof(char *) * (segments + 1));
    if (!arr)
        return (NULL);
    fill_segments(arr, line);
    return (arr);
}
