/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:57 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/25 14:40:24 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

static char	*get_cd_target(char **envp, char **args)
{
	char	*oldpwd;
	char	*home;

	oldpwd = NULL;
	home = NULL;
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
	{
		oldpwd = get_env_value(envp, "OLDPWD");
		if (!oldpwd)
			fprintf(stderr, "cd: OLDPWD not set\n");
		else
			printf("%s\n", oldpwd);
		return (oldpwd);
	}
	if (!args[1] || ft_strlen(args[1]) == 0)
	{
		home = get_env_value(envp, "HOME");
		if (!home)
			fprintf(stderr, "cd: HOME not set\n");
		return (home);
	}
	return (args[1]);
}

static short int	update_pwd_vars(char ***envp, const char *oldpwd,
						const char *newpwd)
{
	char		*old_str;
	char		*new_str;
	char		*export_args[4];
	short int	res;

	old_str = ft_strjoin("OLDPWD=", oldpwd);
	new_str = ft_strjoin("PWD=", newpwd);
	export_args[0] = "export";
	export_args[1] = old_str;
	export_args[2] = new_str;
	export_args[3] = NULL;
	res = 1;
	if (old_str && new_str)
		res = custom_export(envp, export_args);
	free(old_str);
	free(new_str);
	return (res);
}

static short int	change_and_get_pwds(const char *target, char **oldpwd,
						char **newpwd)
{
	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("cd");
		free(*oldpwd);
		*oldpwd = NULL;
		return (1);
	}
	*newpwd = getcwd(NULL, 0);
	if (!*newpwd)
	{
		perror("getcwd");
		free(*oldpwd);
		*oldpwd = NULL;
		return (1);
	}
	return (0);
}

short int	custom_cd(char ***envp, char **args)
{
	char		*target;
	char		*oldpwd;
	char		*newpwd;
	short int	ret;

	target = NULL;
	oldpwd = NULL;
	newpwd = NULL;
	ret = 0;
	if (args[1] && args[2])
		return (fprintf(stderr, "cd: too many arguments\n"), 1);
	target = get_cd_target(*envp, args);
	if (!target)
		return (1);
	ret = change_and_get_pwds(target, &oldpwd, &newpwd);
	if (ret)
		return (ret);
	ret = update_pwd_vars(envp, oldpwd, newpwd);
	free(oldpwd);
	free(newpwd);
	return (ret);
}

