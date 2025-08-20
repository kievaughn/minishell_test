/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:34:05 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/20 12:30:37 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static int      open_heredoc(const char *delim, int quoted, char **envp)
{
        int             pipefd[2];
        char    *line;
        char    *expanded;

        line = NULL;
        if (pipe(pipefd) == -1)
        {
                perror("pipe");
                g_exit_code = 1;
                return (-1);
        }
        while (1)
        {
                line = readline("> ");
                if (!line || ft_strcmp(line, delim) == 0)
                {
                        free(line);
                        break ;
                }
                if (!quoted)
                {
                        expanded = build_expanded_str(line, envp);
                        free(line);
                        if (!expanded)
                        {
                                close(pipefd[1]);
                                return (-1);
                        }
                        write(pipefd[1], expanded, ft_strlen(expanded));
                        free(expanded);
                }
                else
                {
                        write(pipefd[1], line, ft_strlen(line));
                        free(line);
                }
                write(pipefd[1], "\n", 1);
        }
        close(pipefd[1]);
        return (pipefd[0]);
}

int     handle_heredoc(const char *delim, int quoted, char **envp, int *in_fd)
{
        int     h;

        h = open_heredoc(delim, quoted, envp);
        if (h < 0)
                return (-1);
        if (*in_fd != STDIN_FILENO)
                close(*in_fd);
        *in_fd = h;
        return (0);
}

