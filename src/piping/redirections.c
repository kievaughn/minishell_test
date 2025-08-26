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

static void free_token(t_token *tok)
{
    if (!tok)
        return;
    free(tok->str);
    free(tok);
}

static void free_token_array(t_token **arr, int count)
{
    int k = 0;

    if (!arr)
        return;
    while (k < count)
    {
        if (arr[k])
            free_token(arr[k]);
        k++;
    }
    free(arr);
}

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
    char    *filename;
    int     quoted;

    /* not a redirection token */
    if (!(cmd[*i] && (cmd[*i]->type == 1 || cmd[*i]->type == 2
                   || cmd[*i]->type == 3 || cmd[*i]->type == 4)))
        return (0);

    filename = NULL;
    quoted = 0;

    /* validate/collect filename (or allow empty for heredoc) */
    if (cmd[*i + 1] && cmd[*i + 1]->type == 0)
    {
        filename = cmd[*i + 1]->str;
        quoted = (cmd[*i + 1]->quoted != 0);
    }
    else if (!cmd[*i + 1] && cmd[*i]->type != 2)
        return (-1);
    else if (cmd[*i + 1] && cmd[*i + 1]->type != 0)
        return (-1);

    if (cmd[*i]->type == 1) /* < */
    {
        if (!filename || filename[0] == '\0'
            || open_infile(filename, in_fd) == -1)
            return (-1);
    }
    else if (cmd[*i]->type == 2) /* << */
    {
        if (handle_heredoc(filename ? filename : "", quoted, envp, in_fd) == -1)
            return (-1);
    }
    else if (cmd[*i]->type == 3) /* > */
    {
        if (!filename || filename[0] == '\0'
            || open_outfile(filename, out_fd) == -1)
            return (-1);
    }
    else if (cmd[*i]->type == 4) /* >> */
    {
        if (!filename || filename[0] == '\0'
            || open_appendfile(filename, out_fd) == -1)
            return (-1);
    }

    /* consume and free the redirection operator (and its filename if any) */
    free_token(cmd[*i]);
    cmd[*i] = NULL;
    if (cmd[*i + 1])
    {
        free_token(cmd[*i + 1]);
        cmd[*i + 1] = NULL;
    }
    *i += filename ? 2 : 1;
    return (1);
}

t_token **handle_redirections(t_token **cmd, char **envp,
                int *in_fd, int *out_fd)
{
    int i = 0, j = 0;
    int count = count_tokens(cmd);
    t_token **clean = malloc(sizeof(t_token *) * (count + 1));

    if (!clean)
    {
        free_token_array(cmd, count);
        return (NULL);
    }

    while (i < count && cmd[i])
    {
        int res = handle_redirection_logic(cmd, envp, in_fd, out_fd, &i);
        if (res == -1)
        {
            free_token_array(cmd, count);
            free_token_array(clean, j);
            return (NULL);
        }
        else if (res == 0)
        {
            clean[j++] = cmd[i];
            cmd[i] = NULL;
            i++;
        }
        /* res == 1: redirection consumed; continue */
    }

    clean[j] = NULL;
    free(cmd);
    if (j == 0)
    {
        free_token_array(clean, j);
        return (NULL);
    }
    return (clean);
}




