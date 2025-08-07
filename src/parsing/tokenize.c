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
			if (!quote && s[i] == c)
				break ;
			i++;
		}
	}
	return (count);
}

static void	free_arr(char **arr, int i)
{
	while (i-- > 0)
		free(arr[i]);
	free(arr);
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
		if (!quote && s[len] == c)
			break ;
		len++;
	}
	return (len);
}

static int fill_tokens(char **arr, char const **ps, char c, char **envp)
{
    int     i = 0;
    size_t  len;
    char   *tok;
    char   *exp;


    while (**ps)
    {
        while (**ps == c) (*ps)++;
        if (!**ps) break;
        len = next_c((char *)*ps, c);
        tok = ft_substr((char *)*ps, 0, len);
        if (!tok) return (free_arr(arr, i), -1);
        if (tok[0] != '\'')
        {
            exp = build_expanded_str(tok, envp);
            free(tok);
            tok = exp;
        }
        arr[i++] = tok;
        *ps += len;
    }
    return i;
}


char **tokenize_command(char const *s, char c, char **envp)
{
    char **arr;
    int    i;


    if (!s) return NULL;
    arr = malloc((token_count(s, c) + 1) * sizeof *arr);
    if (!arr) return NULL;


    i = fill_tokens(arr, &s, c, envp);
    if (i < 0) return NULL;
    arr[i] = NULL;


    arr = split_redirs(arr);
    if (!arr) return NULL;


    for (i = 0; arr[i]; i++)
        remove_quotes(arr[i]);
    return arr;
}



