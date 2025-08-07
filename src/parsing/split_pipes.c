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

static void	handle_quote_state(char c, char *quote)
{
	if (!*quote && (c == '\'' || c == '"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

char	**split_pipes(const char *line)
{
	size_t	i;
	size_t	start;
	size_t	seg;
	char	quote;
	char	**arr;

	i = 0;
	start = 0;
	seg = 0;
	quote = 0;
	arr = malloc(sizeof(char *) * (count_segments(line) + 1));
	if (!arr)
		return (NULL);
	while (line && line[i])
	{
		handle_quote_state(line[i], &quote);
		if (!quote && line[i] == '|')
		{
			arr[seg++] = ft_substr(line, start, i - start);
			start = i + 1;
		}
		i++;
	}
	arr[seg++] = ft_substr(line, start, i - start);
	arr[seg] = NULL;
	return (arr);
}
