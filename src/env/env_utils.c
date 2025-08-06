/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kievaughn <kievaughn@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:07 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/01 18:08:28 by kievaughn        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

int	env_size(char **env)
{
	int	size;

	size = 0;
	while (env[size])
		size++;
	return (size);
}

char	**env_realloc_add(char **env)
{
	int		size;
	char	**new_env;
	int		i;

	size = env_size(env);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[size] = NULL;
	free(env);
	return (new_env);
}

static int	add_first_env(char ***env_ptr, const char *new_var)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * 2);
	if (!new_env)
		return (-1);
	new_env[0] = ft_strdup(new_var);
	if (!new_env[0])
	{
		free(new_env);
		return (-1);
	}
	new_env[1] = NULL;
	*env_ptr = new_env;
	return (0);
}

int	env_add(char ***env_ptr, const char *new_var)
{
	char	**env;
	int		env_size;
	char	**new_env;

	env = *env_ptr;
	if (!env)
		return (add_first_env(env_ptr, new_var));
	env_size = 0;
	while (env[env_size])
		env_size++;
	new_env = env_realloc_add(env);
	if (!new_env)
		return (-1);
	new_env[env_size] = ft_strdup(new_var);
	if (!new_env[env_size])
	{
		free(new_env);
		return (-1);
	}
	new_env[env_size + 1] = NULL;
	*env_ptr = new_env;
	return (0);
}

char	**copy_envp(char **envp)
{
	char	**env;
	int		size;
	int		i;

	i = -1;
	size = 0;
	while (envp[size])
		size++;
	env = malloc(sizeof(char *) * (size + 1));
	if (!env)
		return (NULL);
	while (++i < size)
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (--i >= 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
	}
	env[size] = NULL;
	return (env);
}
