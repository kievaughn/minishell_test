/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/05 15:46:51 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipeline(char **envp, char **segments)
{
	t_pipeline_data	pipeline;

	pipeline.envp = envp;
	pipeline.segments = segments;
	pipeline.nbr_segments = count_strings(segments);
	pipeline.pids = malloc(sizeof(pid_t) * pipeline.nbr_segments);
	if (!pipeline.pids)
		return ;
	pipeline_loop(&pipeline);
	wait_for_all(pipeline.pids, pipeline.nbr_segments);
	free(pipeline.pids);
}

void	execute_cmd(char **envp, char **cmd)
{
	char	*path;

	if (is_builtin(cmd[0]))
	{
		run_builtin(&envp, cmd);
		free_cmd(cmd);
		exit(g_exit_code);
	}
	path = get_path(envp, cmd);
	if (path)
	{
		execve(path, cmd, envp);
		perror("execve");
		free(path);
		g_exit_code = 126;
	}
	else
	{
		fprintf(stderr, "Command not found: %s\n", cmd[0]);
		g_exit_code = 127;
	}
	free_cmd(cmd);
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

void	wait_for_all(pid_t *pids, int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid");
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				g_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_code = 128 + WTERMSIG(status);
			else
				g_exit_code = 1;
		}
		i++;
	}
}
