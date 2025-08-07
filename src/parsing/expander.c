#include "../libft/libft.h"
#include "minishell.h"

static char	*append_exit_code(char *result, int *i, char *str, int *handled)
{
	char	*exit_code_str;
	char	*tmp;

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

static int	handle_exit_code(char **res, char *s, int *i, int *start)
{
	int	handled;

	*res = append_exit_code(*res, i, s, &handled);
	if (!*res)
		return (-1);
	if (handled)
	{
		*start = *i;
		return (1);
	}
	return (0);
}

static int	handle_env_var(char **res, char *s, int *i, char **envp)
{
	char	*tmp;

	tmp = append_expanded_var(*res, s, i, envp);
	if (!tmp)
		return (-1);
	*res = tmp;
	return (1);
}

static int	process_expansion(char *str, char **envp, char **res)
{
	int	i = 0;
	int	start = 0;
	int	st;

	while (str[i])
	{
		if (str[i] == '$')
		{
			st = handle_exit_code(res, str, &i, &start);
			if (st) { if (st < 0) return (-1); continue ; }
			if (ft_isalnum(str[i + 1]))
			{
				*res = append_literal(*res, str, start, i);
				if (!*res || handle_env_var(res, str, &i, envp) == -1)
					return (-1);
				start = i;
				continue ;
			}
		}
		i++;
	}
	if (!(*res = ft_strcatrealloc(*res, str + start)))
		return (-1);
	return (0);
}

char	*build_expanded_str(char *str, char **envp)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	if (process_expansion(str, envp, &result) == -1)
	{
		free(result);
		return (NULL);
	}
	return (result);
}



