/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    close_pipe(int *fd)
{
    close(fd[0]);
    close(fd[1]);
}

void    parent_cleanup(int *in_fd, int *fd, int i, int num)
{
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    if (i < num - 1)
    {
        close(fd[1]);
        *in_fd = fd[0];
    }
}

void    wait_for_all(pid_t *pids, int count)
{
    int     i;
    int     status;
    int     last_status;
    pid_t   last;

    last_status = -1;
    if (count > 0)
        last = pids[count - 1];
    else
        last = -1;
    i = 0;
    while (i < count)
    {
        if (pids[i] > 0)
        {
            if (waitpid(pids[i], &status, 0) == -1)
                error_perror("waitpid");
            else if (pids[i] == last)
                last_status = status;
        }
        i++;
    }
    if (last_status == -1)
        last_status = status;
    if (WIFEXITED(last_status))
        g_exit_code = WEXITSTATUS(last_status);
    else if (WIFSIGNALED(last_status))
        g_exit_code = 128 + WTERMSIG(last_status);
    else
        g_exit_code = 1;
}

