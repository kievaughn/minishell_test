#include "../libft/libft.h"
#include "minishell.h"

static void	extract_slice(char **out, t_redir *st, char *tok)
{
	if (st->j > st->start || (tok[0] == '\0' && st->j == 0))
	out[st->idx++] = ft_substr(tok, st->start,
				st->j - st->start);
}

static void	handle_operator(char **out, t_redir *st, char *tok)
{
	char	op[2];

	if (tok[st->j] == '>' && tok[st->j + 1] == '>')
	{
		out[st->idx++] = ft_strdup(">>");
		st->j += 2;
	}
	else
	{
	op[0] = tok[st->j];
	op[1] = '\0';
		out[st->idx++] = ft_strdup(op);
		st->j++;
	}
	st->start = st->j;
}

static int	process_redir_token(char **out, char *tok, int idx)
{
	t_redir	st;
	char	quote;

	st.j = 0;
	st.start = 0;
	st.idx = idx;
	quote = 0;
	while (tok[st.j])
	{
		if (!quote && (tok[st.j] == '\'' || tok[st.j] == '"'))
			quote = tok[st.j++];
		else if (quote && tok[st.j] == quote)
			quote = 0, st.j++;
		else if (!quote && (tok[st.j] == '>' || tok[st.j] == '<'))
		{
			extract_slice(out, &st, tok);
			handle_operator(out, &st, tok);
			continue ;
		}
		else
			st.j++;
	}
	extract_slice(out, &st, tok);
	return (st.idx);
}

static int	fill_redirs(char **out, char **arr)
{
	int	i;
	int	idx;

	i = 0;
	idx = 0;
	while (arr[i])
	{
		idx = process_redir_token(out, arr[i], idx);
		free(arr[i]);
		i++;
	}
	return (idx);
}

char	**split_redirs(char **arr)
{
	char	**out;
	int		total;
	int		idx;

	if (!arr)
		return (NULL);
	total = total_parts(arr);
	out = malloc((total + 1) * sizeof *out);
	if (!out)
		return (free_cmd(arr), NULL);
	idx = fill_redirs(out, arr);
	out[idx] = NULL;
	free(arr);
	return (out);
}

