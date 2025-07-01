#include "minishell.h"
#include "../libft/libft.h"

static size_t	token_count(char const *s, char c)
{
        size_t  i;
        size_t  count;
        char    quote;
		
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
static size_t   next_c(char *s, char c)
{
    size_t  len;
	char    quote;

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
static void     remove_quotes(char *str)
{
        size_t  i;
        size_t  j;
        char    quote;

        i = 0;
        j = 0;
        quote = 0;
        while (str[i])
        {
                if (!quote && (str[i] == '"' || str[i] == '\''))
                {
                        quote = str[i++];
                        while (str[i] && str[i] != quote)
                                str[j++] = str[i++];
                        if (str[i] == quote)
                                i++;
                        quote = 0;
                }
                else
                        str[j++] = str[i++];
        }
        str[j] = '\0';
}

static char *append_literal(char *result, char *str, int start, int i)
{
    char *tmp;

    str[i] = '\0';
    tmp = ft_strcatrealloc(result, str + start);
    str[i] = '$';
    if (!tmp)
    {
        free(result);
        return (NULL);
    }
    return (tmp);
}

static char *expand_var(char *str, int *var_len)
{
    int i = 0;

    while (str[i] != '\0' && ft_isalnum(str[i]))
        i++;
    *var_len = i;
    if (i > 0)
        return (ft_substr(str, 0, i));
    return (NULL);
}

static char *append_expanded_var(char *result, char *str, int *i, char **envp)
{
    int var_len;
    char *var;
    char *value;
    char *tmp;

    var_len = 0;
    var = expand_var(&str[*i + 1], &var_len);
    value = get_env_value(envp, var);
    if (!value)
        value = "";
    tmp = ft_strcatrealloc(result, value);
    free(var);
    if (!tmp)
        return (NULL);
    *i += var_len + 1;
    return (tmp);
}

char *append_exit_code(char *result, int *i, char *str, int *handled)
{
    char *exit_code_str;
    char *tmp;

    *handled = 0;

    if (!(str[*i] == '$' && str[*i + 1] == '?'))
        return (result);

    *handled = 1;

    if (!(result = append_literal(result, str, 0, *i)))
        return (NULL);

    exit_code_str = ft_itoa(last_exit_code);
    if (!exit_code_str)
        return (free(result), NULL);

    tmp = ft_strcatrealloc(result, exit_code_str);
    free(exit_code_str);
    if (!tmp)
        return (free(result), NULL);

    result = tmp;
    *i += 2;
    return (result);
}

char *build_expanded_str(char *str, char **envp)
{
    int     i = 0;
    int     start = 0;
    char    *result = NULL;
    int     handled;

    while (str[i])
    {
        if (str[i] == '$')
        {
            if ((result = append_exit_code(result, &i, str, &handled)) && handled)
            {
                start = i;
                continue;
            }
            if (ft_isalnum(str[i + 1]))
            {
                if (!(result = append_literal(result, str, start, i)))
                    return (NULL);
                if (!(result = append_expanded_var(result, str, &i, envp)))
                    return (NULL);
                start = i;
                continue;
            }
        }
        i++;
    }
    if (!(result = ft_strcatrealloc(result, str + start)))
        return (NULL);

    return (result);
}

char    **ft_tokenize(char const *s, char c, char **envp)
{
        char    **arr;
        int             i;
        size_t  len;

        if (!s)
                return (NULL);
        arr = (char **)malloc((token_count(s, c) + 1) * sizeof(char *));
        if (!arr)
                return (NULL);
        i = 0;
        while (*s)
        {
                while (*s == c)
                        s++;
                if (!*s)
                        break ;
                len = next_c((char *)s, c);
                arr[i] = ft_substr((char *)s, 0, len);
                if (!arr[i])
                        return (free_arr(arr, i), NULL);
                if(arr[i][0] != '\'')
                {
                        char *expanded = build_expanded_str(arr[i], envp);
                        free(arr[i]);
                        arr[i] = expanded;
                }
                remove_quotes(arr[i]);
                i++;
                s += len;
        }
        arr[i] = NULL;
        return (arr);
}
