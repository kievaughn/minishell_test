#include "../../libft/libft.h"
#include "minishell.h"

#define SPACE_MARK 1
#define GREAT_MARK 2
#define LESS_MARK 3

static void mark_quoted_chars(char *str)
{
    char    quote;
    size_t  i;

    quote = 0;
    i = 0;
    while (str[i])
    {
        if (!quote && (str[i] == '\'' || str[i] == '"'))
            quote = str[i];
        else if (quote && str[i] == quote)
            quote = 0;
        else if (quote)
        {
            if (str[i] == ' ')
                str[i] = SPACE_MARK;
            else if (str[i] == '>')
                str[i] = GREAT_MARK;
            else if (str[i] == '<')
                str[i] = LESS_MARK;
        }
        i++;
    }
}

void    remove_quotes(char *str)
{
    char    quote;
    size_t  i;
    size_t  j;

    mark_quoted_chars(str);

    quote = 0;
    i = 0;
    j = 0;
    while (str[i])
    {
        if (!quote && (str[i] == '\'' || str[i] == '"'))
            quote = str[i++];
        else if (quote && str[i] == quote)
        {
            quote = 0;
            i++;
        }
        else
            str[j++] = str[i++];
    }
    str[j] = '\0';
}

void    restore_marked_chars(char *str)
{
    size_t  i;

    i = 0;
    while (str[i])
    {
        if (str[i] == SPACE_MARK)
            str[i] = ' ';
        else if (str[i] == GREAT_MARK)
            str[i] = '>';
        else if (str[i] == LESS_MARK)
            str[i] = '<';
        i++;
    }
}

char    *append_literal(char *result, char *str, int start, int i)
{
    char    *tmp;

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

char    *expand_var(char *str, int *var_len)
{
    int     i;

    i = 0;
    while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    *var_len = i;
    if (i > 0)
        return (ft_substr(str, 0, i));
    return (NULL);
}

char    *append_expanded_var(char *result, char *str, int *i, char **envp)
{
    int             var_len;
    char            *var;
    char            *value;
    char            *tmp;

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

