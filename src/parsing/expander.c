#include "../libft/libft.h"
#include "minishell.h"

static char     *append_exit_code(char *result, int *i, char *str, int *handled)
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

static int      handle_exit_code_case(char **result, char *str, int *i, int *start)
{
        int     handled;

        *result = append_exit_code(*result, i, str, &handled);
        if (*result && handled)
        {
                *start = *i;
                return (1);
        }
        return (0);
}

static int      handle_env_var_case(char **result, char *str, int *i, int *start,
                char **envp)
{
        if (ft_isalnum(str[*i + 1]))
        {
                if (!(*result = append_literal(*result, str, *start, *i)))
                        return (-1);
                if (!(*result = append_expanded_var(*result, str, i, envp)))
                        return (-1);
                *start = *i;
                return (1);
        }
        return (0);
}

char    *build_expanded_str(char *str, char **envp)
{
        int             i;
        int             start;
        char            quote;
        char            *result;
        int             env_result;

        i = 0;
        start = 0;
        quote = 0;
        result = NULL;
        while (str[i])
        {
                if (!quote && (str[i] == '\'' || str[i] == '"'))
                        quote = str[i++];
                else if (quote && str[i] == quote)
                        quote = 0, i++;
                else if (str[i] == '$' && quote != '\'')
                {
                        if (handle_exit_code_case(&result, str, &i, &start))
                                continue ;
                        env_result = handle_env_var_case(&result, str, &i, &start, envp);
                        if (env_result == -1)
                                return (NULL);
                        else if (env_result == 1)
                                continue ;
                        i++;
                }
                else
                        i++;
        }
        if (!(result = ft_strcatrealloc(result, str + start)))
                return (NULL);
        return (result);
}

