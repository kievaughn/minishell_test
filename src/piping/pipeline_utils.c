/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 16:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"
#include <errno.h>

static void cleanup_commands(t_pipeline_data *pipeline, int count)
{
    int i = 0;
    while (i < count)
    {
        if (pipeline->cmds[i].in_fd != STDIN_FILENO)
            close(pipeline->cmds[i].in_fd);
        if (pipeline->cmds[i].out_fd != STDOUT_FILENO)
            close(pipeline->cmds[i].out_fd);
        free_tokens(pipeline->cmds[i].tokens);
        i++;
    }
    free(pipeline->cmds);
}

void    execute_pipeline(char **envp, char **segments)
{
    t_pipeline_data pipeline;
    int             i;

    pipeline.envp = envp;
    pipeline.segments = segments;
    pipeline.nbr_segments = count_strings(segments);
    pipeline.cmds = ft_calloc(pipeline.nbr_segments, sizeof(t_command));
    if (!pipeline.cmds)
    {
        g_exit_code = 1;
        return ;
    }
    i = 0;
    while (i < pipeline.nbr_segments)
    {
        pipeline.cmds[i].in_fd = STDIN_FILENO;
        pipeline.cmds[i].out_fd = STDOUT_FILENO;
        pipeline.cmds[i].cmd_type = 0;
        pipeline.cmds[i].has_heredoc = 0;
        pipeline.cmds[i].tokens = tokenize_command(segments[i], ' ', envp);
        if (!pipeline.cmds[i].tokens)
        {
            cleanup_commands(&pipeline, i);
            g_exit_code = 1;
            return ;
        }
        int t = 0;
        while (pipeline.cmds[i].tokens[t])
        {
            if (pipeline.cmds[i].tokens[t]->type == 2)
                pipeline.cmds[i].has_heredoc = 1;
            t++;
        }
        pipeline.cmds[i].tokens = handle_redirections(pipeline.cmds[i].tokens,
                envp, &pipeline.cmds[i].in_fd, &pipeline.cmds[i].out_fd);
        if (!pipeline.cmds[i].tokens)
        {
            if (pipeline.cmds[i].in_fd != STDIN_FILENO)
                close(pipeline.cmds[i].in_fd);
            if (pipeline.cmds[i].out_fd != STDOUT_FILENO)
                close(pipeline.cmds[i].out_fd);
            pipeline.cmds[i].in_fd = STDIN_FILENO;
            pipeline.cmds[i].out_fd = STDOUT_FILENO;
            pipeline.cmds[i].cmd_type = 0;
            i++;
            continue;
        }
        if (pipeline.cmds[i].tokens[0])
            pipeline.cmds[i].cmd_type = is_builtin(pipeline.cmds[i].tokens[0]->str);
        i++;
    }
    pipeline.pids = ft_calloc(pipeline.nbr_segments, sizeof(pid_t));
    if (!pipeline.pids)
    {
        cleanup_commands(&pipeline, pipeline.nbr_segments);
        g_exit_code = 1;
        return ;
    }
    pipeline_loop(&pipeline);
    wait_for_all(pipeline.pids, pipeline.nbr_segments);
    cleanup_commands(&pipeline, pipeline.nbr_segments);
    free(pipeline.pids);
}
static int      exit_code_from_errno(void)
{
        if (errno == ENOENT)
                return (127);
        return (126);
}

char **tokens_to_argv(t_token **cmd)
{
    int count = 0;
    while (cmd && cmd[count])
        count++;

    char **argv = malloc((count + 1) * sizeof(char *));
    if (!argv)
        return NULL;

    for (int i = 0; i < count; i++)
        argv[i] = ft_strdup(cmd[i]->str); // duplicate string from token

    argv[count] = NULL;
    return argv;
}

void execute_cmd(char **envp, t_token **cmd)
{
    char *path;

    char **argv = tokens_to_argv(cmd);
    if (!argv || !argv[0])
    {
        free_cmd(argv);
        free_tokens(cmd);
        exit(0);
    }

    // Builtins
    if (is_builtin(argv[0]))
    {
        run_builtin(&envp, cmd);
        free_cmd(argv);
        free_tokens(cmd);
        exit(g_exit_code);
    }

    // Path with slash
    if (ft_strchr(argv[0], '/'))
    {
        if (access(argv[0], F_OK) != 0)
        {
            perror(argv[0]);
            g_exit_code = 127;
        }
        else if (is_folder(argv[0]))
        {
            fprintf(stderr, "%s: Is a directory\n", argv[0]);
            g_exit_code = 126;
        }
        else if (access(argv[0], X_OK) != 0)
        {
            perror(argv[0]);
            g_exit_code = 126;
        }
        else
        {
            execve(argv[0], argv, envp);
            perror(argv[0]);
            g_exit_code = exit_code_from_errno();
        }
    }
    else
    {
        path = get_path(envp, argv);
        if (path)
        {
            execve(path, argv, envp);
            perror(argv[0]);
            free(path);
            g_exit_code = exit_code_from_errno();
        }
        else
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            g_exit_code = 127;
        }
    }

    free_cmd(argv);
    free_tokens(cmd);
    exit(g_exit_code);
}

void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void	parent_cleanup(int *in_fd, int *fd, int i, int num)
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
        last = (count > 0) ? pids[count - 1] : -1;
        i = 0;
        while (i < count)
        {
                if (pids[i] > 0)
                {
                        if (waitpid(pids[i], &status, 0) == -1)
                                perror("waitpid");
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

