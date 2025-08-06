/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/05 16:22:49 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <errno.h>

int	open_infile(char *file, int *in_fd)
{
	int	fd;

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

int	open_outfile(char *file, int *out_fd)
{
	int	fd;

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

int	open_appendfile(char *file, int *out_fd)
{
	int	fd;

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

int	handle_redirection_logic(char **cmd, int *in_fd, int *out_fd, int *i)
{
	if (!ft_strcmp(cmd[*i], "<") && cmd[*i + 1])
	{
		if (open_infile(cmd[*i + 1], in_fd) == -1)
			return (-1);
	}
	else if (!ft_strcmp(cmd[*i], "<<") && cmd[*i + 1])
	{
		if (handle_heredoc(cmd[*i + 1], in_fd) == -1)
			return (-1);
	}
	else if (!ft_strcmp(cmd[*i], ">") && cmd[*i + 1])
	{
		if (open_outfile(cmd[*i + 1], out_fd) == -1)
			return (-1);
	}
	else if (!ft_strcmp(cmd[*i], ">>") && cmd[*i + 1])
	{
		if (open_appendfile(cmd[*i + 1], out_fd) == -1)
			return (-1);
	}
	else
		return (0);
	(*i) += 2;
	return (1);
}

char	**handle_redirections(char **cmd, int count, int *in_fd, int *out_fd)
{
	int		i;
	int		j;
	char	**clean;
	int		res;

	i = 0;
	j = 0;
	clean = malloc(sizeof(char *) * (count + 1));
	if (!clean)
		return (NULL);
	while (cmd[i])
	{
		res = handle_redirection_logic(cmd, in_fd, out_fd, &i);
		if (res == -1)
		{
			free(clean);
			return (NULL);
		}
		else if (res == 0)
			clean[j++] = cmd[i++];
	}
	clean[j] = NULL;
	free(cmd);
	return (clean);
}
