#include "../libft/libft.h"
#include "minishell.h"

static int	part_count(char *tok)
{
	int	i;
	int	start;
	int	count;

	i = 0;
	start = 0;
	count = 0;
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

static int	total_parts(char **arr)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (arr && arr[i])
	{
		total += part_count(arr[i]);
		i++;
	}
	return (total);
}

char	**split_redirs(char **arr)
{
	char	**out;
	int		i;
	int		j;
	int		start;
	int		idx;
	char	quote;
	char	op[2];

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
