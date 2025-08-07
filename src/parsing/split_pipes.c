#include "../libft/libft.h"
#include "minishell.h"

static size_t	count_segments(const char *line)
{
	size_t	i;
	size_t	count;
	char	quote;

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

static int	add_segment(char **arr, size_t *seg,
						const char *start, size_t len)
{
	arr[*seg] = ft_substr(start, 0, len);
	if (!arr[*seg])
		return (0);
	(*seg)++;
	return (1);
}

static int	fill_pipes(char **arr, const char *line)
{
	size_t	i;
	size_t	start;
	size_t	seg;
	char	quote;

	i = 0;
	start = 0;
	seg = 0;
	quote = 0;
	while (line && line[i])
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		if (!quote && line[i] == '|')
		{
			if (!add_segment(arr, &seg, line + start, i - start))
				return (-1);
			start = i + 1;
		}
		i++;
	}
	return (add_segment(arr, &seg, line + start, i - start) ? (int)seg : -1);
}

char	**split_pipes(const char *line)
{
	char	**arr;
	int		seg_total;

	arr = malloc(sizeof(char *) * (count_segments(line) + 1));
	if (!arr)
		return (NULL);
	seg_total = fill_pipes(arr, line);
	if (seg_total == -1)
		return (free_cmd(arr), NULL);
	arr[seg_total] = NULL;
	return (arr);
}

