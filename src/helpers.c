/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:07 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/16 16:00:02 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

short int is_builtin(const char *cmd)
{
    return (
        ft_strncmp(cmd, "echo", 5) == 0 ||
        ft_strncmp(cmd, "cd", 3) == 0 ||
        ft_strncmp(cmd, "pwd", 4) == 0 ||
        ft_strncmp(cmd, "export", 7) == 0
/*        ft_strncmp(cmd, "unset", 6) == 0 ||
        ft_strncmp(cmd, "env", 4) == 0 ||
        ft_strncmp(cmd, "exit", 5) == 0 */
    );
}

void execute_command(char *path, char **cmd, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
    status = 0;
    if (pid == 0)
    {
        if (execve(path, cmd, envp) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}

char **copy_envp(char **envp)
{
    char **env;
    int size;
    int i;

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


