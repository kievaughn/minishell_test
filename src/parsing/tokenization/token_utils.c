#include "../../libft/libft.h"
#include "token_utils.h"

int	fully_quoted(const char *s)
{
	size_t	len;
	size_t	i;
	char	quote;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 2)
		return (0);
	quote = s[0];
	if ((quote != '\'' && quote != '"') || s[len - 1] != quote)
		return (0);
	i = 1;
	while (i < len - 1)
	{
		if (s[i] == quote)
			return (0);
		i++;
	}
	if (quote == '\'')
		return (1);
	return (2);
}

int	is_delim(char ch, char delim)
{
	if (delim == ' ')
		return (ch == ' ' || ch == '\t');
	return (ch == delim);
}

size_t	skip_token(const char *s, size_t i, char c)
{
	char	quote;

	quote = 0;
	while (s[i])
	{
		if (!quote && (s[i] == '"' || s[i] == '\''))
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				i++;
			if (s[i] == quote)
				i++;
			quote = 0;
			continue ;
		}
		if (!quote && is_delim(s[i], c))
			break ;
		i++;
	}
	return (i);
}

size_t	token_count(const char *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (is_delim(s[i], c))
			i++;
		if (!s[i])
			break ;
		count++;
		i = skip_token(s, i, c);
	}
	return (count);
}

size_t	next_c(const char *s, char c)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i])
	{
		if (!quote && (s[i] == '"' || s[i] == '\''))
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				i++;
			if (s[i] == quote)
				i++;
			quote = 0;
			continue ;
		}
		if (!quote && is_delim(s[i], c))
			break ;
		i++;
	}
	return (i);
}
