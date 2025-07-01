#include "minishell.h"
#include "../libft/libft.h"

static char    *append_exit_code(char *result, int *i, char *str, int *handled)
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

char    *build_expanded_str(char *str, char **envp)
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
