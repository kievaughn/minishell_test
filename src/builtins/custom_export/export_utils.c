#include "minishell.h"
#include "../libft/libft.h"

int     is_valid_name(const char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	sort_index(char **env, int *index, int size)
{
	int	i;
	int	j;
	int	aux;
	int	len;

	i = -1;
	while (++i < size - 1)
	{
		j = i;
		while (++j < size)
		{
			if (ft_strlen(env[index[i]]) > ft_strlen(env[index[j]]))
				len = ft_strlen(env[index[i]]);
			else
				len = ft_strlen(env[index[j]]);
			if (ft_strncmp(env[index[i]], env[index[j]], len) > 0)
			{
				aux = index[i];
				index[i] = index[j];
				index[j] = aux;
			}
		}
	}
}

void	init_export_index(int *index, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		index[i] = i;
		i++;
	}
}