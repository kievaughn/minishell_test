/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/05 17:41:44 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

char	*get_env_value(char **envp, const char *name)
{
	int		i;
	size_t	len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	**split_path_dirs(char **envp, const char *name)
{
	char	*value;

	value = get_env_value(envp, name);
	if (!value)
		return (NULL);
	return (ft_split(value, ':'));
}

char	*check_and_construct_path(char *path, char **cmd)
{
	char	*tmp;

	tmp = NULL;
	tmp = ft_strcatrealloc(tmp, path);
	tmp = ft_strcatrealloc(tmp, "/");
	tmp = ft_strcatrealloc(tmp, cmd[0]);
	if (tmp && access(tmp, F_OK | X_OK) == 0)
	{
		return (ft_strdup(tmp));
	}
	free(tmp);
	return (NULL);
}

char	*get_path(char **envp, char **cmd)
{
	int		i;
	char	**paths;
	char	*finalpath;

	finalpath = NULL;
	paths = split_path_dirs(envp, "PATH");
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		finalpath = check_and_construct_path(paths[i], cmd);
		if (finalpath)
			break ;
		i++;
	}
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (finalpath);
}
