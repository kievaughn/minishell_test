/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:57 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/23 19:47:10 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

short int custom_cd(char **envp, char **args)
{
    char *target_dir;

    if (args[1] && args[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return (1);
    }
    
    if (!args[1] || ft_strlen(args[1]) == 0)
    {
        target_dir = get_env_value(envp, "HOME");
        if (!target_dir)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }
    else
        target_dir = args[1];

    if (chdir(target_dir) != 0)
    {
        perror("cd");
        return 1;
    }
    //PWD and OLDPWD here
    return (0);
}
