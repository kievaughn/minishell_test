/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:15:02 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 12:42:30 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>

int     is_folder(char *arg)
{
        int     fd_test;

        fd_test = open(arg, O_DIRECTORY);
        if (fd_test != -1)
        {
                close(fd_test);
                return (126);
        }
        return (0);
}

t_token **prepare_command(char *segment, int *in_fd, int *out_fd, char ***envp)
{
    t_token **cmd;
    
    cmd = tokenize_command(segment, ' ', *envp);
    if (!cmd)
        return (NULL);

    cmd = handle_redirections(cmd, *envp, in_fd, out_fd);
    if (!cmd || !cmd[0])
    {
        if (cmd)
            free_tokens(cmd);
        if (*in_fd != STDIN_FILENO)
        {
            close(*in_fd);
            *in_fd = STDIN_FILENO;
        }
        if (*out_fd != STDOUT_FILENO)
        {
            close(*out_fd);
            *out_fd = STDOUT_FILENO;
        }
        return (NULL);
    }
    return (cmd);
}

void    setup_redirections(int in_fd, int out_fd, int *save_in, int *save_out)
{
        *save_in = -1;
        *save_out = -1;

        if (in_fd != STDIN_FILENO)
        {
                *save_in = dup(STDIN_FILENO);
                if (*save_in == -1)
                        perror("dup");
                if (dup2(in_fd, STDIN_FILENO) == -1)
                        perror("dup2");
                close(in_fd);
        }

        if (out_fd != STDOUT_FILENO)
        {
                *save_out = dup(STDOUT_FILENO);
                if (*save_out == -1)
                        perror("dup");
                if (dup2(out_fd, STDOUT_FILENO) == -1)
                        perror("dup2");
                close(out_fd);
        }
}

void    restore_redirections(int save_in, int save_out)
{
        if (save_in != -1)
        {
                if (dup2(save_in, STDIN_FILENO) == -1)
                        perror("dup2");
                close(save_in);
        }

        if (save_out != -1)
        {
                if (dup2(save_out, STDOUT_FILENO) == -1)
                        perror("dup2");
                close(save_out);
        }
}

short int       is_builtin(const char *cmd)
{
        return (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0
                || ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0
                || ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0
                || ft_strncmp(cmd, "exit", 5) == 0);
}

