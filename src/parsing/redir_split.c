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
				i += 2;
			else
				i++;
			count++;
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

static void	handle_redir(char **out, char *tok, int *j, int *idx, int *start)
{
	char	op[2];

	if (*j - *start > 0)
	{
		out[*idx] = ft_substr(tok, *start, *j - *start);
		(*idx)++;
	}
	if (tok[*j] == '>' && tok[*j + 1] == '>')
	{
		out[*idx] = ft_strdup(">>");
		(*idx)++;
		*j += 2;
	}
	else
	{
		op[0] = tok[*j];
		op[1] = '\0';
		out[*idx] = ft_strdup(op);
		(*idx)++;
		(*j)++;
	}
	*start = *j;
}

static void	process_token(char **out, char *tok, int *idx)
{
	int	j;
	int	start;
	char	quote;

	j = 0;
	start = 0;
	quote = 0;
	while (tok[j])
	{
		if (!quote && (tok[j] == '\'' || tok[j] == '"'))
			quote = tok[j];
		else if (quote && tok[j] == quote)
			quote = 0;
		else if (!quote && (tok[j] == '>' || tok[j] == '<'))
		{
			handle_redir(out, tok, &j, idx, &start);
			continue ;
		}
		j++;
	}
	if (j - start > 0)
	{
		out[*idx] = ft_substr(tok, start, j - start);
		(*idx)++;
	}
}

char	**split_redirs(char **arr)
{
	char	**out;
	int	i;
	int	idx;

	out = malloc(sizeof(char *) * (total_parts(arr) + 1));
	if (!out)
	{
		free_cmd(arr);
		return (NULL);
	}
	idx = 0;
	i = 0;
	while (arr[i])
	{
		process_token(out, arr[i], &idx);
		free(arr[i]);
		i++;
	}
	out[idx] = NULL;
	free(arr);
	return (out);
}
