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

static char     **split_ifs(char *s)
{
        char    *tmp;
        char    **parts;
        int             i;

        tmp = ft_strdup(s);
        if (!tmp)
                return (NULL);
        i = 0;
        while (tmp[i])
        {
                if (tmp[i] == '\t' || tmp[i] == '\n')
                        tmp[i] = ' ';
                i++;
        }
        parts = ft_split(tmp, ' ');
        free(tmp);
        return (parts);
}

static int      has_quote(char *s)
{
        int i;

        i = 0;
        while (s[i])
        {
                if (s[i] == '\'' || s[i] == '"')
                        return (1);
                i++;
        }
        return (0);
}

static int      ensure_capacity(char ***arr, int *cap, int needed, int used)
{
        char    **tmp;
        int             newcap;

        if (used + needed + 1 <= *cap)
                return (0);
        newcap = *cap * 2;
        while (used + needed + 1 > newcap)
                newcap *= 2;
        tmp = realloc(*arr, newcap * sizeof *tmp);
        if (!tmp)
                return (-1);
        *arr = tmp;
        *cap = newcap;
        return (0);
}

static int      fill_tokens(char ***arr, char const **ps, char c, char **envp, int *cap)
{
    int     i;
    size_t  len;
    char   *tok;
    char   *exp;
    char  **parts;
    int     k;

    i = 0;
    while (**ps)
    {
        while (**ps == c)
            (*ps)++;
        if (!**ps)
            break ;
        len = next_c((char *)*ps, c);
        tok = ft_substr((char *)*ps, 0, len);
        if (!tok)
            return (free_arr(*arr, i), -1);
        if (tok[0] != '\'')
        {
            exp = build_expanded_str(tok, envp);
            free(tok);
            tok = exp;
            if (!tok)
                return (free_arr(*arr, i), -1);
            if (!has_quote(tok))
            {
                parts = split_ifs(tok);
                free(tok);
                if (!parts)
                    return (free_arr(*arr, i), -1);
                k = 0;
                while (parts[k])
                    k++;
                if (ensure_capacity(arr, cap, k, i) < 0)
                    return (free_cmd(parts), free_arr(*arr, i), -1);
                k = 0;
                while (parts[k])
                    (*arr)[i++] = parts[k++];
                free(parts);
            }
            else
            {
                if (ensure_capacity(arr, cap, 1, i) < 0)
                    return (free(tok), free_arr(*arr, i), -1);
                (*arr)[i++] = tok;
            }
        }
        else
        {
            if (ensure_capacity(arr, cap, 1, i) < 0)
                return (free(tok), free_arr(*arr, i), -1);
            (*arr)[i++] = tok;
        }
        *ps += len;
    }
    return (i);
}


char **tokenize_command(char const *s, char c, char **envp)
{
    char **arr;
    int    i;
    int    cap;

    if (!s)
        return NULL;
    cap = token_count(s, c) + 1;
    arr = malloc(cap * sizeof *arr);
    if (!arr)
        return NULL;

    i = fill_tokens(&arr, &s, c, envp, &cap);
    if (i < 0)
        return NULL;
    arr[i] = NULL;


    arr = split_redirs(arr);
    if (!arr) return NULL;


    for (i = 0; arr[i]; i++)
        remove_quotes(arr[i]);
    return arr;
}



