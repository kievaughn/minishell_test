/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:57 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:55:26 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

t_token *make_token(char *str)
{
    t_token *tok = malloc(sizeof(t_token));
    tok->str = ft_strdup(str);
    tok->type = 0;
    return tok;
}

static char	*get_cd_target(char **envp, t_token **args)
{
	char	*oldpwd;
	char	*home;

	oldpwd = NULL;
	home = NULL;
	if (args[1] && ft_strncmp(args[1]->str, "-", 2) == 0)
	{
		oldpwd = get_env_value(envp, "OLDPWD");
		if (!oldpwd)
			fprintf(stderr, "cd: OLDPWD not set\n");
		else
			printf("%s\n", oldpwd);
		return (oldpwd);
	}
	if (!args[1] || ft_strlen(args[1]->str) == 0)
	{
		home = get_env_value(envp, "HOME");
		if (!home)
			fprintf(stderr, "cd: HOME not set\n");
		return (home);
	}
	return (args[1]->str);
}

static short int	update_pwd_vars(char ***envp, const char *oldpwd,
		const char *newpwd)
{
	char		*old_str;
	char		*new_str;
	t_token		*export_args[4];
	short int	res;

	old_str = ft_strjoin("OLDPWD=", oldpwd);
	new_str = ft_strjoin("PWD=", newpwd);

	export_args[0] = make_token("export");   // you need a helper that mallocs + sets ->str
	export_args[1] = make_token(old_str);
	export_args[2] = make_token(new_str);
	export_args[3] = NULL;

	res = 1;
	if (old_str && new_str)
		res = custom_export(envp, export_args);

	free(old_str);
	free(new_str);
	free_tokens(export_args); // free the temporary t_token* array
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

short int	custom_cd(char ***envp, t_token **args)
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

