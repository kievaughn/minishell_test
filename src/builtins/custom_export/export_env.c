#include "minishell.h"
#include "../libft/libft.h"

static int	compare_var_names(char *a, char *b, int size)
{
	if (ft_strncmp(a, b, size) == 0
		&& (a[size] == '=' || a[size] == '\0'))
		return (1);
	return (0);
}

static int	replace_env(char ***env, int i, char *arg)
{
	free((*env)[i]);
	(*env)[i] = ft_strdup(arg);
	return (0);
}

static int	add_env_var(char ***env, char *arg)
{
	if (env_add(env, arg) == -1)
		return (-1);
	return (0);
}

static int	update_existing_env(char ***env, char *arg, int i, int size)
{
	if (arg[size] == '=')
		return (replace_env(env, i, arg));
	return (0);
}

int	update_or_add_env(char ***env, char *arg)
{
	int	i;
	int	size;
	int	found;

	i = 0;
	found = 0;
	size = 0;
	while (arg[size] && arg[size] != '=')
		size++;
	while ((*env)[i])
	{
		if (compare_var_names((*env)[i], arg, size))
		{
			update_existing_env(env, arg, i, size);
			found = 1;
			break ;
		}
		i++;
	}
	if (!found)
		return (add_env_var(env, arg));
	return (0);
}
