/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 16:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static void	cleanup_commands(t_pipeline *pipeline, int count)
{
	int	i;

	i = 0;
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

void	execute_pipeline(char **envp, char **segments)
{
	t_pipeline	pipeline;
	int			i;
	int			t;

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
		t = 0;
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
			continue ;
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
