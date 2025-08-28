#include "../../libft/libft.h"
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

static void	update_quote(char c, char *quote, int *i)
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

static int	process_dollar(char **res, char *str, int *i, int *start,
		char **envp)
{
	int	handled;

	*res = append_exit_code(*res, i, str, &handled);
	if (!*res)
		return (-1);
	if (handled)
	{
		*start = *i;
		return (1);
	}
	if (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
	{
		if (!(*res = append_literal(*res, str, *start, *i)))
			return (-1);
		if (!(*res = append_expanded_var(*res, str, i, envp)))
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
	char	quote;
	char	*result;
	int		handled;

	i = 0;
	start = 0;
	quote = 0;
	result = NULL;
	while (str[i])
	{
		if (!quote && str[i] == '$' && (str[i + 1] == '"'))
		{
			if (!(result = append_literal(result, str, start, i)))
				return (NULL);
			i++;
			start = i;
			continue ;
		}
		handled = i;
		update_quote(str[i], &quote, &i);
		if (i != handled)
			continue ;
		if (str[i] == '$' && quote != '\'')
		{
			handled = process_dollar(&result, str, &i, &start, envp);
			if (handled == -1)
				return (NULL);
			if (handled)
				continue ;
		}
		i++;
	}
	if (!(result = ft_strcatrealloc(result, str + start)))
		return (NULL);
	return (result);
}
