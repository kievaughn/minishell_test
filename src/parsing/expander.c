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
	result = append_literal(result, str, 0, *i);
	if (!result)
		return (NULL);
	exit_code_str = ft_itoa(g_exit_code);
	if (!exit_code_str)
	{
		free(result);
		return (NULL);
	}
	tmp = ft_strcatrealloc(result, exit_code_str);
	free(exit_code_str);
	if (!tmp)
	{
		free(result);
		return (NULL);
	}
	result = tmp;
	*i += 2;
	return (result);
}

static int	handle_dollar(char **result, char *str, int *i,
		int *start, char **envp)
{
	int	handled;

	if (str[*i] != '$')
		return (0);
	*result = append_exit_code(*result, i, str, &handled);
	if (!*result)
		return (-1);
	if (handled)
	{
		*start = *i;
		return (1);
	}
	if (ft_isalnum(str[*i + 1]))
	{
		*result = append_literal(*result, str, *start, *i);
		if (!*result)
			return (-1);
		*result = append_expanded_var(*result, str, i, envp);
		if (!*result)
			return (-1);
		*start = *i;
		return (1);
	}
	return (0);
}

char	*build_expanded_str(char *str, char **envp)
{
	int		i;
	int		start;
	char	*result;
	int		status;

	i = 0;
	start = 0;
	result = NULL;
	while (str[i])
	{
		status = handle_dollar(&result, str, &i, &start, envp);
		if (status == -1)
			return (NULL);
		if (status == 1)
			continue ;
		i++;
	}
	result = ft_strcatrealloc(result, str + start);
	if (!result)
		return (NULL);
	return (result);
}
