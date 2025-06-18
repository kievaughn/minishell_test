/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:22:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/18 17:51:47 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int execute_command(char *path, char **cmd, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execve(path, cmd, envp) == -1)
        {
            perror("execve");
            _exit(127);
        }
    }
    else if (pid > 0)
    {
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            return 1;
        }

        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);
        else
            return 1;
    }
    else
    {
        perror("fork");
        return 1;
    }
        return 1;
}
