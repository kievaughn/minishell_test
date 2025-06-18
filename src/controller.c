/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:22:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/18 19:09:28 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int run_builtin(char ***envp, char **cmd)
{
    if (!ft_strncmp(cmd[0], "echo", 5))
        last_exit_code = custom_echo(cmd);
    else if (!ft_strncmp(cmd[0], "cd", 3))
        last_exit_code = custom_cd(*envp, cmd);
    else if (!ft_strncmp(cmd[0], "pwd", 4))
        last_exit_code = custom_pwd();
    else if (!ft_strncmp(cmd[0], "export", 7))
        last_exit_code = custom_export(envp, cmd);
    else if (!ft_strncmp(cmd[0], "unset", 6))
        last_exit_code = custom_unset(envp, cmd);
    else if (!ft_strncmp(cmd[0], "env", 4))
        last_exit_code = custom_env(*envp);
    else if (!ft_strncmp(cmd[0], "exit", 5))
    {
        last_exit_code = custom_exit(cmd);
        if (last_exit_code != 1)
            exit(last_exit_code);
    }
    else
        last_exit_code = 127;

    return (last_exit_code);
}

short int is_builtin(const char *cmd)
{
    return (
        ft_strncmp(cmd, "echo", 5) == 0 ||
        ft_strncmp(cmd, "cd", 3) == 0 ||
        ft_strncmp(cmd, "pwd", 4) == 0 ||
        ft_strncmp(cmd, "export", 7) == 0 ||
        ft_strncmp(cmd, "unset", 6) == 0 ||
        ft_strncmp(cmd, "env", 4) == 0 ||
        ft_strncmp(cmd, "exit", 5) == 0
    );
}

void process_command(char ***envp, char *line)
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
        path = get_path(*envp, cmd);
        if (path)
        {
            last_exit_code = execute_command(path, cmd, *envp);
            free(path);
        }
        else
        {
            printf("Command not found: %s\n", cmd[0]);
            last_exit_code = 127;
        }
    }
    free_cmd(cmd);
}
