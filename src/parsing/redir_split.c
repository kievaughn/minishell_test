#include "../libft/libft.h"
#include "minishell.h"

typedef struct s_redir
{
	int		j;
	int		idx;
	int		start;
}	t_redir;

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
				i++;
			count++;
			i++;
			start = i;
			continue ;
		}
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

static void	handle_redir(char **out, char *tok, t_redir *st)
{
	char	op[3];

	if (st->j - st->start > 0)
	{
		out[st->idx] = ft_substr(tok, st->start, st->j - st->start);
		st->idx++;
	}
	if (tok[st->j] == '>' && tok[st->j + 1] == '>')
	{
		out[st->idx] = ft_strdup(">>");
		st->j += 2;
	}
	else
	{
		op[0] = tok[st->j];
		op[1] = '\0';
		out[st->idx] = ft_strdup(op);
		st->j++;
	}
	st->idx++;
	st->start = st->j;
}

static void	process_token(char **out, char *tok, int *idx)
{
	t_redir	st;
	char	quote;

	st.j = 0;
	st.start = 0;
	st.idx = *idx;
	quote = 0;
	while (tok[st.j])
	{
		if (!quote && (tok[st.j] == '\'' || tok[st.j] == '"'))
			quote = tok[st.j];
		else if (quote && tok[st.j] == quote)
			quote = 0;
		else if (!quote && (tok[st.j] == '>' || tok[st.j] == '<'))
		{
			handle_redir(out, tok, &st);
			continue ;
		}
		st.j++;
	}
	if (st.j - st.start > 0)
	{
		out[st.idx] = ft_substr(tok, st.start, st.j - st.start);
		st.idx++;
	}
	*idx = st.idx;
}

char	**split_redirs(char **arr)
{
	char	**out;
	int		i;
	int		idx;

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

