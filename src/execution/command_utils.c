/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:15:02 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/27 12:10:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <fcntl.h>

static t_token **parse_segment(char *segment, char ***envp)
{
    t_token **cmd;

    cmd = tokenize_command(segment, ' ', *envp);
    if (!cmd || !cmd[0])
    {
        if (cmd)
            free_tokens(cmd);
        return (NULL);
    }
    return (cmd);
}

static int  apply_redirs(t_token ***cmd, char ***envp, int *in_fd, int *out_fd)
{
    t_token **clean;

    clean = handle_redirections(*cmd, *envp, in_fd, out_fd);
    if (!clean || !clean[0])
    {
        if (clean)
            free_tokens(clean);
        *cmd = NULL;
        return (1);
    }
    *cmd = clean;
    return (0);
}

static void reset_fds(int *in_fd, int *out_fd)
{
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
}

int is_folder(char *arg)
{
    int fd_test;

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

    cmd = parse_segment(segment, envp);
    if (!cmd)
        return (NULL);
    if (apply_redirs(&cmd, envp, in_fd, out_fd))
    {
        reset_fds(in_fd, out_fd);
        return (NULL);
    }
    return (cmd);
}

void setup_redirections(int in_fd, int out_fd, int *save_in, int *save_out)
{
    *save_in = -1;
    *save_out = -1;
    if (in_fd != STDIN_FILENO)
    {
        *save_in = dup(STDIN_FILENO);
        if (*save_in == -1)
            error_perror("dup");
        if (dup2(in_fd, STDIN_FILENO) == -1)
            error_perror("dup2");
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO)
    {
        *save_out = dup(STDOUT_FILENO);
        if (*save_out == -1)
            error_perror("dup");
        if (dup2(out_fd, STDOUT_FILENO) == -1)
            error_perror("dup2");
        close(out_fd);
    }
}

void restore_redirections(int save_in, int save_out)
{
    if (save_in != -1)
    {
        if (dup2(save_in, STDIN_FILENO) == -1)
            error_perror("dup2");
        close(save_in);
    }
    if (save_out != -1)
    {
        if (dup2(save_out, STDOUT_FILENO) == -1)
            error_perror("dup2");
        close(save_out);
    }
}

short int       is_builtin(const char *cmd)
{
        if (!cmd)
                return (0);
        /*
         * Reject names containing any whitespace so quoted commands like
         * "echo " are executed externally instead of dispatching to builtins.
         */
        if (ft_strchr(cmd, ' ') || ft_strchr(cmd, '\t')
                || ft_strchr(cmd, '\n') || ft_strchr(cmd, '\r')
                || ft_strchr(cmd, '\v') || ft_strchr(cmd, '\f'))
                return (0);
        return (!ft_strcmp((char *)cmd, "echo")
                || !ft_strcmp((char *)cmd, "cd")
                || !ft_strcmp((char *)cmd, "pwd")
                || !ft_strcmp((char *)cmd, "export")
                || !ft_strcmp((char *)cmd, "unset")
                || !ft_strcmp((char *)cmd, "env")
                || !ft_strcmp((char *)cmd, "exit"));
}


