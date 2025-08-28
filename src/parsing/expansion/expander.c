#include "../../libft/libft.h"
#include "minishell.h"

static char *append_exit_code(char *result, int *i, char *str, int *handled)
{
    char    *exit_code_str;
    char    *tmp;

    *handled = 0;
    if (!(str[*i] == '$' && str[*i + 1] == '?'))
        return (result);
    *handled = 1;
    if (!(result = append_literal(result, str, 0, *i)))
        return (NULL);
    exit_code_str = ft_itoa(g_exit_code);
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

static void update_quote(char c, char *quote, int *i)
{
    if (!*quote && (c == '\'' || c == '"'))
    {
        *quote = c;
        (*i)++;
    }
    else if (*quote && c == *quote)
    {
        *quote = 0;
        (*i)++;
    }
}

static char *collect_var_name(char *str, int *i)
{
    int     j;
    char    quote;
    char    *name;
    char    tmp[2];

    j = *i + 1;
    quote = 0;
    name = NULL;
    while (str[j])
    {
        if (!quote && (str[j] == '\'' || str[j] == '"'))
        {
            quote = str[j++];
            continue;
        }
        if (quote && str[j] == quote)
        {
            quote = 0;
            j++;
            continue;
        }
        if (ft_isalnum(str[j]) || str[j] == '_')
        {
            tmp[0] = str[j++];
            tmp[1] = '\0';
            name = ft_strcatrealloc(name, tmp);
            if (!name)
                return (NULL);
            continue;
        }
        break;
    }
    *i = j;
    return (name);
}

static int process_dollar(char **res, char *str, int *i, int *start, char **envp)
{
    int     handled;
    char    *name;
    char    *value;
    char    *tmp;

    *res = append_exit_code(*res, i, str, &handled);
    if (!*res && handled)
        return (-1);
    if (handled)
    {
        *start = *i;
        return (1);
    }
    if (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_' || str[*i + 1] == '\'' || str[*i + 1] == '"')
    {
        if (!(*res = append_literal(*res, str, *start, *i)))
            return (-1);
        name = collect_var_name(str, i);
        if (!name)
            return (-1);
        value = get_env_value(envp, name);
        free(name);
        if (!value)
            value = "";
        tmp = ft_strcatrealloc(*res, value);
        if (!tmp)
            return (-1);
        *res = tmp;
        *start = *i;
        return (1);
    }
    return (0);
}

char *build_expanded_str(char *str, char **envp)
{
    int     i;
    int     start;
    char    quote;
    char    *result;
    int     handled;

    i = 0;
    start = 0;
    quote = 0;
    result = NULL;
    while (str[i])
    {
        handled = i;
        update_quote(str[i], &quote, &i);
        if (i != handled)
            continue;
        if (str[i] == '$' && quote != '\'')
        {
            handled = process_dollar(&result, str, &i, &start, envp);
            if (handled == -1)
                return (NULL);
            if (handled)
                continue;
        }
        i++;
    }
    if (!(result = ft_strcatrealloc(result, str + start)))
        return (NULL);
    return (result);
}
