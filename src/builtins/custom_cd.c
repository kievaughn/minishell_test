/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:57 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/13 18:20:29 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

short int custom_cd(char **envp, char **args)
{
    char *target_dir;

	target_dir = NULL;
    if (!args[1] || ft_strlen(args[1]) == 0)
    {
        target_dir = get_env_value(envp, "HOME");
        if (!target_dir)
        {
            printf("cd: HOME not set\n");
            return (1);
        }
    }
    else
    {
        target_dir = args[1];
    }
    if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (1);
	}
    return (0);
}
