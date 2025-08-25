/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:33 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:23:18 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void     handle_input_redirection(int redir_in, int *in_fd)
{
        if (redir_in != STDIN_FILENO)
        {
                if (redir_in < 0)
                {
                        perror("Input redirection");
                        exit(1);
                }
                dup2(redir_in, STDIN_FILENO);
                close(redir_in);
                if (*in_fd != STDIN_FILENO)
                {
                        close(*in_fd);
                        *in_fd = STDIN_FILENO;
                }
        }
        else if (*in_fd != STDIN_FILENO)
        {
                dup2(*in_fd, STDIN_FILENO);
                close(*in_fd);
        }
}

static void     handle_output_redirection(int redir_out, int *fd, int last)
{
        if (!last && redir_out == STDOUT_FILENO)
        {
                dup2(fd[1], STDOUT_FILENO);
        }
        else if (redir_out != STDOUT_FILENO)
        {
                if (redir_out < 0)
                {
                        perror("Output redirection");
                        exit(1);
                }
                dup2(redir_out, STDOUT_FILENO);
        }
}

static void child_process(char **envp, char *segment, t_pipe_info pipe_info)
{
    int redir_in = STDIN_FILENO;
    int redir_out = STDOUT_FILENO;

    // Tokenize input line
    t_token **cmd = tokenize_command(segment, ' ', envp);
    if (!cmd)
        exit(1);

    // Handle <, >, <<, >>
    cmd = handle_redirections(cmd, envp, &redir_in, &redir_out);
    if (!cmd)
        exit(1);

    // Setup input/output redirection
    handle_input_redirection(redir_in, &pipe_info.in_fd);
    handle_output_redirection(redir_out, pipe_info.fd, pipe_info.last);

    if (!pipe_info.last)
    {
        close(pipe_info.fd[0]);
        close(pipe_info.fd[1]);
    }

    // Execute command
    execute_cmd(envp, cmd);

    free_tokens(cmd);
    exit(0); // make sure child exits
}

static int pipeline_step(t_pipeline_data *pipeline, int *in_fd, int *fd, int i)
{
    t_pipe_info pipe_info;

    if (i < pipeline->nbr_segments - 1 && pipe(fd) == -1)
    {
        perror("pipe");
        return (0);
    }

    pipe_info.in_fd = *in_fd;
    pipe_info.fd = fd;
    pipe_info.last = (i == pipeline->nbr_segments - 1);

    pipeline->pids[i] = fork();
    if (pipeline->pids[i] == 0)
    {
        // Child takes care of tokenizing the segment
        child_process(pipeline->envp, pipeline->segments[i], pipe_info);
    }

    // Parent cleanup
    parent_cleanup(in_fd, fd, i, pipeline->nbr_segments);
    return (1);
}


void pipeline_loop(t_pipeline_data *pipeline)
{
    int in_fd = STDIN_FILENO;
    int fd[2];
    int i = 0;

    while (i < pipeline->nbr_segments)
    {
        if (!pipeline_step(pipeline, &in_fd, fd, i))
            break;
        i++;
    }
    if (in_fd != STDIN_FILENO)
        close(in_fd);
}

