/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:30:28 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <errno.h>

int count_tokens(t_token **arr)
{
    int i = 0;
    while (arr && arr[i])
        i++;
    return i;
}

int     open_infile(char *file, int *in_fd)
{
        int     fd;

        fd = 0;
        if (*in_fd != STDIN_FILENO)
                close(*in_fd);
        fd = open(file, O_RDONLY);
        if (fd < 0)
        {
                perror(file);
                if (errno == EACCES)
                        g_exit_code = 126;
                else if (errno == ENOENT)
                        g_exit_code = 127;
                else
                        g_exit_code = 1;
                return (-1);
        }
        *in_fd = fd;
        return (0);
}

int     open_outfile(char *file, int *out_fd)
{
        int     fd;

        fd = 0;
        if (*out_fd != STDOUT_FILENO)
                close(*out_fd);
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
                perror(file);
                if (errno == EACCES)
                        g_exit_code = 126;
                else
                        g_exit_code = 1;
                return (-1);
        }
        *out_fd = fd;
        return (0);
}

int     open_appendfile(char *file, int *out_fd)
{
        int     fd;

        fd = 0;
        if (*out_fd != STDOUT_FILENO)
                close(*out_fd);
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
        {
                perror(file);
                if (errno == EACCES)
                        g_exit_code = 126;
                else
                        g_exit_code = 1;
                return (-1);
        }
        *out_fd = fd;
        return (0);
}

static int handle_redirection_logic(t_token **cmd, char **envp,
                                    int *in_fd, int *out_fd, int *i)
{
    char *filename;

    if (cmd[*i]->type == 1 || cmd[*i]->type == 2
        || cmd[*i]->type == 3 || cmd[*i]->type == 4)
    {
        if (!cmd[*i + 1] || cmd[*i + 1]->type != 0)
            return (-1);

        filename = cmd[*i + 1]->str;

        if (cmd[*i]->type == 1)
        {
            if (open_infile(filename, in_fd) == -1)
                return (-1);
        }
        else if (cmd[*i]->type == 2)
        {
            // use token->type instead of quoted[]
            int quoted = (cmd[*i + 1]->type == 11
                       || cmd[*i + 1]->type == 22);
            if (handle_heredoc(filename, quoted, envp, in_fd) == -1)
                return (-1);
        }
        else if (cmd[*i]->type == 3)
        {
            if (open_outfile(filename, out_fd) == -1)
                return (-1);
        }
        else if (cmd[*i]->type == 4)
        {
            if (open_appendfile(filename, out_fd) == -1)
                return (-1);
        }
        *i += 2;
        return (1);
    }
    return (0);
}


t_token **handle_redirections(t_token **cmd, char **envp,
                int *in_fd, int *out_fd)
{
    int i = 0, j = 0;
    t_token **clean;
    int count = count_tokens(cmd);

    clean = malloc(sizeof(t_token *) * (count + 1));
    if (!clean)
        return (NULL);

    while (cmd[i])
    {
        int res = handle_redirection_logic(cmd, envp, in_fd, out_fd, &i);
        if (res == -1)
        {
            free_tokens(clean);
            return (NULL);
        }
        else if (res == 0)
            clean[j++] = cmd[i++];
    }
    clean[j] = NULL;
    free(cmd);
    if (j == 0)
    {
        free(clean);
        return (NULL);
    }
    return clean;
}




