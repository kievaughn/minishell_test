/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:22:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/13 18:22:39 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void process_command(char **envp, char *line)
{
    char **cmd;
    char *path;
    cmd = ft_tokenize(line, ' ');
    
    if (!cmd || !cmd[0])
    {
        free_cmd(cmd);
        return;
    }

    if (is_builtin(cmd[0]))
        run_builtin(envp, cmd);
    else
    {
        path = get_path(envp, cmd);
        if (path)
        {
            execute_command(path, cmd, envp);
            free(path);
        }
        else
            printf("Command not found: %s\n", cmd[0]);
    }

    free_cmd(cmd);
}
