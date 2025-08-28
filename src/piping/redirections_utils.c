/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:34:05 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 10:21:54 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <signal.h>

int open_heredoc_child(int write_fd, const char *delimiter, int quoted, char **envp)
{
    char *line;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
    while (1)
    {
        line = readline("> ");
        if (!line || !ft_strcmp(line, delimiter))
        {
            free(line);
            break;
        }
        if (!quoted)
        {
            char *expanded = build_expanded_str(line, envp);
            free(line);
            line = expanded;
            if (!line)
                continue;
        }
        write(write_fd, line, ft_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
    close(write_fd);
    exit(0);
}

int open_heredoc(int write_fd, const char *delimiter, int quoted, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
        return (-1);
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_IGN);
        open_heredoc_child(write_fd, delimiter, quoted, envp);
    }
    close(write_fd);
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        g_exit_code = 130;
        return (-1);
    }
    return (0);
}

int handle_heredoc(const char *delim, int quoted, char **envp, int *in_fd)
{
    int fd[2];

    if (pipe(fd) == -1)
        return (-1);

    if (open_heredoc(fd[1], delim, quoted, envp) == -1)
    {
        close(fd[0]);
        return (-1);
    }

    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    *in_fd = fd[0];

    return (0);
}




