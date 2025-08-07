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
			quote = s[len++];
			while (s[len] && s[len] != quote)
				len++;
			if (s[len] == quote)
				len++;
			quote = 0;
			continue ;
		}
		if (s[len] == c)
			break ;
		len++;
	}
	return (len);
}

static char	*get_token(char const **s, char c, char **envp)
{
	size_t	len;
	char	*tok;
	char	*expanded;

	len = next_c((char *)*s, c);
	tok = ft_substr((char *)*s, 0, len);
	if (!tok)
		return (NULL);
	/* only expand if not a single-quoted literal */
	if (tok[0] != '\'')
	{
		expanded = build_expanded_str(tok, envp);
		free(tok);
		tok = expanded;
	}
	*s += len;
	return (tok);
}

