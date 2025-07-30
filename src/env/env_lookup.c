/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/08 17:11:48 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

char *get_env_value(char **envp, const char *name)
{
    int i;
    size_t len;
	
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

static char **split_path_dirs(char **envp, const char *name)
{
    char *value;
	
	value = get_env_value(envp, name);
    if (!value)
        return (NULL);
    return (ft_split(value, ':'));
}

char *get_path(char **envp, char **cmd)
{
    int i;
    char **paths;
    char *finalpath;
    char *tmp;

    finalpath = NULL;
    paths = split_path_dirs(envp, "PATH");
    if (!paths)
        return NULL;

    i = 0;
    while (paths[i])
    {
        tmp = NULL;
        tmp = ft_strcatrealloc(tmp, paths[i]);
        tmp = ft_strcatrealloc(tmp, "/");
        tmp = ft_strcatrealloc(tmp, cmd[0]);
        if (tmp && access(tmp, F_OK | X_OK) == 0)
        {
            finalpath = ft_strdup(tmp);
            free(tmp);
            break;
        }
        free(tmp);
        i++;
    }
    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);

    return (finalpath);
}

