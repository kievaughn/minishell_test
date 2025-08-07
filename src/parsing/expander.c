#include "../libft/libft.h"
#include "minishell.h"

typedef struct s_expand
{
	char	*str;
	char	**envp;
	char	*result;
	int		start;
}	t_expand;

static char	*append_exit_code(char *result, char *str, int *i)
{
	char	*exit_code_str;
	char	*tmp;

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

static int	handle_dollar(t_expand *d, int *i)
{
	if (d->str[*i] != '$')
		return (0);
	if (d->str[*i + 1] == '?')
	{
		d->result = append_exit_code(d->result, d->str, i);
		if (!d->result)
			return (-1);
		d->start = *i;
		return (1);
	}
	if (ft_isalnum(d->str[*i + 1]))
	{
		d->result = append_literal(d->result, d->str, d->start, *i);
		if (!d->result)
			return (-1);
		d->result = append_expanded_var(d->result, d->str, i, d->envp);
		if (!d->result)
			return (-1);
		d->start = *i;
		return (1);
	}
	return (0);
}

char	*build_expanded_str(char *str, char **envp)
{
	int			i;
	int			status;
	t_expand	d;

	i = 0;
	d.str = str;
	d.envp = envp;
	d.result = NULL;
	d.start = 0;
	while (str[i])
	{
		status = handle_dollar(&d, &i);
		if (status == -1)
			return (NULL);
		if (status == 1)
			continue ;
		i++;
	}
	d.result = ft_strcatrealloc(d.result, str + d.start);
	if (!d.result)
		return (NULL);
	return (d.result);
}

