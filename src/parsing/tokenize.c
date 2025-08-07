#include "../libft/libft.h"
#include "minishell.h"

static size_t	token_count(char const *s, char c)
{
	size_t	i;
	size_t	count;
	char	quote;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break ;
		count++;
		quote = 0;
		while (s[i] && (quote || s[i] != c))
		{
			if (!quote && (s[i] == '\'' || s[i] == '"'))
				quote = s[i];
			else if (quote && s[i] == quote)
				quote = 0;
			i++;
		}
	}
	return (count);
}

static size_t	next_c(char *s, char c)
{
	size_t	len;
	char	quote;

	len = 0;
	quote = 0;
	while (s[len])
	{
		if (!quote && (s[len] == '"' || s[len] == '\''))
		{
			quote = s[len];
			len++;
			while (s[len] && s[len] != quote)
				len++;
			if (s[len] == quote)
				len++;
			quote = 0;
			continue ;
		}
		if (!quote && s[len] == c)
			break ;
		len++;
	}
	return (len);
}

static char *get_token(char const **s, char c, char **envp)
{
	size_t	len;
	char	*tok;
	char	*expanded;

	len = next_c((char *)*s, c);
	tok = ft_substr((char *)*s, 0, len);
	if (!tok)
		return (NULL);
	if (tok[0] != '\'')
	{
		expanded = build_expanded_str(tok, envp);
		free(tok);
		tok = expanded;
	}
	*s += len;
	return (tok);
}

static int	fill_tokens(char **arr, char const *s, char c, char **envp)
{
	int		i;
	char	*tok;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (!*s)
			break ;
		tok = get_token(&s, c, envp);
		if (!tok)
		{
			while (i > 0)
			{
				i--;
				free(arr[i]);
			}
			free(arr);
			return (-1);
		}
		arr[i] = tok;
		i++;
	}
	arr[i] = NULL;
	return (i);
}

char	**tokenize_command(char const *s, char c, char **envp)
{
	char	**arr;
	int		i;

	if (!s)
		return (NULL);
	arr = (char **)malloc((token_count(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = fill_tokens(arr, s, c, envp);
	if (i < 0)
		return (NULL);
	arr = split_redirs(arr);
	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		remove_quotes(arr[i]);
		i++;
	}
	return (arr);
}
