/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:15:10 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/30 15:17:17 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>

int	run_builtin(char ***envp, char **cmd)
{
	if (!ft_strncmp(cmd[0], "echo", 5))
		g_exit_code = custom_echo(cmd);
	else if (!ft_strncmp(cmd[0], "cd", 3))
		g_exit_code = custom_cd(envp, cmd);
	else if (!ft_strncmp(cmd[0], "pwd", 4))
		g_exit_code = custom_pwd();
	else if (!ft_strncmp(cmd[0], "export", 7))
		g_exit_code = custom_export(envp, cmd);
	else if (!ft_strncmp(cmd[0], "unset", 6))
		g_exit_code = custom_unset(envp, cmd);
	else if (!ft_strncmp(cmd[0], "env", 4))
		g_exit_code = custom_env(*envp);
	else if (!ft_strncmp(cmd[0], "exit", 5))
	{
		g_exit_code = custom_exit(cmd);
		if (g_exit_code != 1)
			exit(g_exit_code);
	}
	else
		g_exit_code = 127;
	return (g_exit_code);
}

static void	execute_command_with_path(char **cmd, char ***envp)
{
	char	*path;

	path = NULL;
	path = get_path(*envp, cmd);
	if (path)
	{
		g_exit_code = execute_command(path, cmd, *envp);
		free(path);
	}
	else
	{
		fprintf(stderr, "%s: command not found\n", cmd[0]);
		g_exit_code = 127;
	}
}

static void	run_external_command(char **cmd, char ***envp)
{
	if (ft_strchr(cmd[0], '/'))
	{
		if (access(cmd[0], F_OK) != 0)
		{
			perror(cmd[0]);
			g_exit_code = 127;
		}
		else if (is_folder(cmd[0]))
		{
			fprintf(stderr, "%s: Is a directory\n", cmd[0]);
			g_exit_code = 126;
		}
		else if (access(cmd[0], X_OK) != 0)
		{
			perror(cmd[0]);
			g_exit_code = 126;
		}
		else
			g_exit_code = execute_command(cmd[0], cmd, *envp);
	}
	else
		execute_command_with_path(cmd, envp);
}

static void	run_single(char ***envp, char *segment)
{
	char	**cmd;
	int		in_fd;
	int		out_fd;
	int		save_in;
	int		save_out;

	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	save_in = 0;
	save_out = 0;
	cmd = prepare_command(segment, &in_fd, &out_fd, envp);
	if (!cmd)
		return ;
	setup_redirections(in_fd, out_fd, &save_in, &save_out);
	if (is_builtin(cmd[0]))
		run_builtin(envp, cmd);
	else
		run_external_command(cmd, envp);
	restore_redirections(in_fd, out_fd, save_in, save_out);
	free_cmd(cmd);
}

void	process_command(char ***envp, char *line)
{
	char	**segments;
	int		count;

	segments = split_pipes(line);
	if (!segments)
		return ;
	count = count_strings(segments);
	if (count > 1)
		execute_pipeline(*envp, segments);
	else
		run_single(envp, segments[0]);
	free_cmd(segments);
}
