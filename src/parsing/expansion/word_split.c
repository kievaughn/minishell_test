#include "../../libft/libft.h"
#include "minishell.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

static int	count_words(const char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (is_whitespace(s[i]))
			i++;
		if (!s[i])
			break ;
		count++;
		while (s[i] && !is_whitespace(s[i]))
			i++;
	}
	return (count);
}

static void	fill_words(char **out, const char *s)
{
	int	i;
	int	k;
	int	start;

	i = 0;
	k = 0;
	while (s[i])
	{
		while (is_whitespace(s[i]))
			i++;
		if (!s[i])
			break ;
		start = i;
		while (s[i] && !is_whitespace(s[i]))
			i++;
		out[k++] = ft_substr(s, start, i - start);
	}
	out[k] = NULL;
}

static char	**split_whitespace(const char *s)
{
	char	**out;
	int		count;

	count = count_words(s);
	out = malloc(sizeof(char *) * (count + 1));
	if (!out)
		return (NULL);
	fill_words(out, s);
	return (out);
}

static int	compute_total(t_token **arr, int *total)
{
	int		i;
	char	**parts;

	i = 0;
	*total = 0;
	while (arr && arr[i])
	{
		if (arr[i]->quoted)
			(*total)++;
		else
		{
			parts = split_whitespace(arr[i]->str);
			if (!parts)
				return (free_tokens(arr), 1);
			*total += count_strings(parts);
			free_cmd(parts);
		}
		i++;
	}
	return (0);
}

static int	expand_token(t_token **out, t_token *tok, int *k)
{
	char	**parts;
	int		j;

	if (tok->quoted)
	{
		out[(*k)++] = tok;
		return (0);
	}
	parts = split_whitespace(tok->str);
	free(tok->str);
	free(tok);
	if (!parts)
		return (1);
	j = 0;
	while (parts[j])
		out[(*k)++] = new_token(parts[j++], 0, 0);
	free(parts);
	return (0);
}

t_token	**split_expanded_tokens(t_token **arr)
{
	int		i;
	int		k;
	int		total;
	t_token	**out;

	if (compute_total(arr, &total))
		return (NULL);
	out = malloc(sizeof(t_token *) * (total + 1));
	if (!out)
		return (free_tokens(arr), NULL);
	i = 0;
	k = 0;
	while (arr && arr[i])
	{
		if (expand_token(out, arr[i], &k))
		{
			free_tokens(out);
			free_tokens(arr + i + 1);
			free(arr);
			return (NULL);
		}
		i++;
	}
	out[k] = NULL;
	free(arr);
	return (out);
}
