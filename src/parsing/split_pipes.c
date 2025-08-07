#include "../libft/libft.h"
#include "minishell.h"

typedef struct s_pipe
{
	char	**arr;
	size_t	seg;
}	t_pipe;

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

static void handle_quote_state(char c, char *quote)
{
	if (!*quote && (c == '\'' || c == '"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

static void add_segment(t_pipe *p, const char *line, size_t start, size_t end)
{
	p->arr[p->seg] = ft_substr(line, start, end - start);
	p->seg++;
}

char	**split_pipes(const char *line)
{
	size_t	i;
	size_t	start;
	char	quote;
	t_pipe	p;

	i = 0;
	start = 0;
	quote = 0;
	p.seg = 0;
	p.arr = malloc(sizeof(char *) * (count_segments(line) + 1));
	if (!p.arr)
		return (NULL);
	while (line && line[i])
	{
		handle_quote_state(line[i], &quote);
		if (!quote && line[i] == '|')
		{
			add_segment(&p, line, start, i);
			start = i + 1;
		}
		i++;
	}
	add_segment(&p, line, start, i);
	p.arr[p.seg] = NULL;
	return (p.arr);
}
