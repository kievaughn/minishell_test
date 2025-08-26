/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:20:00 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 14:20:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>

extern int  g_exit_code;

/* ======================= TTY HANDLING ======================= */

int tty_open(t_tty *tty)
{
    if (!tty)
        return (-1);
    tty->in_fd = open("/dev/tty", O_RDONLY | O_CLOEXEC);
    if (tty->in_fd < 0)
        tty->in_fd = dup(STDIN_FILENO);
    if (tty->in_fd < 0)
        return (-1);
    tty->out_fd = open("/dev/tty", O_WRONLY | O_CLOEXEC);
    if (tty->out_fd < 0)
        tty->out_fd = dup(STDOUT_FILENO);
    if (tty->out_fd < 0)
    {
        close(tty->in_fd);
        return (-1);
    }
    return (0);
}

void tty_close(t_tty *tty)
{
    if (!tty)
        return ;
    if (tty->in_fd >= 0)
        close(tty->in_fd);
    if (tty->out_fd >= 0)
        close(tty->out_fd);
    tty->in_fd = -1;
    tty->out_fd = -1;
}

void tty_push_readline(const t_tty *tty, const char *prompt, char **out_line)
{
    FILE    *saved_in;
    FILE    *saved_out;
    FILE    *in;
    FILE    *out;

    if (!out_line)
        return ;
    saved_in = rl_instream;
    saved_out = rl_outstream;
    in = fdopen(dup(tty->in_fd), "r");
    out = fdopen(dup(tty->out_fd), "w");
    rl_instream = in;
    rl_outstream = out;
    *out_line = readline(prompt);
    rl_instream = saved_in;
    rl_outstream = saved_out;
    if (in)
        fclose(in);
    if (out)
        fclose(out);
}

/* ======================= EXPANSION ======================= */

static const char *lookup_env(const char *name, char **envp)
{
    size_t  len;
    int     i;

    len = strlen(name);
    i = 0;
    while (envp && envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return ("");
}

int expand_line_for_heredoc(char **line, int do_expand, int last_status, char **envp)
{
    char        *src;
    char        *res;
    size_t      i;
    size_t      r;
    int         in_single;

    if (!line || !*line)
        return (0);
    src = *line;
    if (!do_expand)
        return (0);
    res = malloc(strlen(src) * 2 + 20);
    if (!res)
        return (-1);
    i = 0;
    r = 0;
    in_single = 0;
    while (src[i])
    {
        if (src[i] == '\'' && !in_single)
        {
            in_single = 1;
            i++;
            continue ;
        }
        if (src[i] == '\'' && in_single)
        {
            in_single = 0;
            i++;
            continue ;
        }
        if (src[i] == '$' && !in_single)
        {
            if (src[i + 1] == '?')
            {
                char buf[12];
                int  len;

                snprintf(buf, sizeof(buf), "%d", last_status);
                len = strlen(buf);
                memcpy(res + r, buf, len);
                r += len;
                i += 2;
            }
            else if (isalpha((unsigned char)src[i + 1]) || src[i + 1] == '_')
            {
                size_t start = i + 1;
                while (isalnum((unsigned char)src[i + 1]) || src[i + 1] == '_')
                    i++;
                char save = src[i + 1];
                src[i + 1] = '\0';
                const char *val = lookup_env(src + start, envp);
                size_t len = strlen(val);
                memcpy(res + r, val, len);
                r += len;
                src[i + 1] = save;
                i++;
            }
            else
                res[r++] = src[i++];
        }
        else
            res[r++] = src[i++];
    }
    res[r] = '\0';
    free(*line);
    *line = res;
    return (0);
}

/* ======================= HEREDOC ======================= */

static volatile sig_atomic_t *g_sigint_ptr;

static void heredoc_sighandler(int sig)
{
    (void)sig;
    if (g_sigint_ptr)
        *g_sigint_ptr = 1;
    rl_done = 1;
}

int collect_heredoc(const t_tty *tty, t_heredoc *hd,
                    volatile sig_atomic_t *sigint_flag)
{
    int                 fd;
    char                template[] = "/tmp/minish-XXXXXX";
    char                *line;
    struct sigaction    sa_new;
    struct sigaction    sa_old;
    extern char         **environ;

    if (!hd || !sigint_flag)
        return (-1);
    fd = mkstemp(template);
    if (fd < 0)
        return (-1);
    hd->temp_path = strdup(template);
    if (!hd->temp_path)
    {
        close(fd);
        unlink(template);
        return (-1);
    }
    g_sigint_ptr = sigint_flag;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sa_new.sa_handler = heredoc_sighandler;
    sigaction(SIGINT, &sa_new, &sa_old);
    while (!*sigint_flag)
    {
        tty_push_readline(tty, "heredoc> ", &line);
        if (!line)
            break ;
        if (strcmp(line, hd->delim) == 0)
        {
            free(line);
            break ;
        }
        expand_line_for_heredoc(&line, !hd->quoted, g_exit_code, environ);
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    sigaction(SIGINT, &sa_old, NULL);
    close(fd);
    if (*sigint_flag)
    {
        unlink(hd->temp_path);
        free(hd->temp_path);
        hd->temp_path = NULL;
        g_exit_code = 130;
        return (-1);
    }
    return (0);
}

int handle_heredoc(const t_tty *tty, const char *delim, int quoted,
                   char **envp, int *in_fd)
{
    t_heredoc               hd;
    volatile sig_atomic_t   sigint_flag;

    (void)envp;
    sigint_flag = 0;
    hd.delim = (char *)delim;
    hd.quoted = quoted;
    hd.temp_path = NULL;
    if (collect_heredoc(tty, &hd, &sigint_flag) == -1)
        return (-1);
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    *in_fd = open(hd.temp_path, O_RDONLY);
    unlink(hd.temp_path);
    free(hd.temp_path);
    if (*in_fd < 0)
        return (-1);
    return (0);
}

/* ======================================================================= */

