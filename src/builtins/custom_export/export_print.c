#include "minishell.h"

static void	print_one_env(char *var)
{
	int	i;

	i = 0;
	write(1, "declare -x ", 11);
	while (var[i] && var[i] != '=')
	{
		write(1, &var[i], 1);
		i++;
	}
	if (var[i] == '=')
	{
		write(1, "=\"", 2);
		i++;
		while (var[i])
		{
			write(1, &var[i], 1);
			i++;
		}
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

void	print_env(char **env, int *index, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		print_one_env(env[index[i]]);
		i++;
	}
}
