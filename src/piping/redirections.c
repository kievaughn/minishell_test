/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 16:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <errno.h>

static void	free_token(t_token *tok)
{
	if (!tok)
		return ;
	free(tok->str);
	free(tok);
}

static void	free_token_array(t_token **arr, int count)
{
	int	k;

	k = 0;
	if (!arr)
		return ;
	while (k < count)
	{
		if (arr[k])
			free_token(arr[k]);
		k++;
	}
	free(arr);
}

static int	count_tokens(t_token **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

static t_token	**alloc_clean_array(int count)
{
	t_token	**clean;

	clean = malloc(sizeof(t_token *) * (count + 1));
	if (!clean)
		return (NULL);
	return (clean);
}

static int	apply_infile(char *filename, int *in_fd)
{
	if (!filename || filename[0] == '\0' || open_infile(filename, in_fd) == -1)
		return (-1);
	return (0);
}

static int	apply_heredoc(char *filename, int quoted, char **envp, int *in_fd)
{
	const char	*name;

	if (filename)
		name = filename;
	else
		name = "";
	if (handle_heredoc(name, quoted, envp, in_fd) == -1)
		return (-1);
	return (0);
}

static int	apply_outfile(char *filename, int *out_fd)
{
	if (!filename || filename[0] == '\0' || open_outfile(filename, out_fd) ==
		-1)
		return (-1);
	return (0);
}

static int	apply_append(char *filename, int *out_fd)
{
	if (!filename || filename[0] == '\0' || open_appendfile(filename, out_fd) ==
		-1)
		return (-1);
	return (0);
}

static int	fetch_filename(t_token **cmd, int *i, char **filename, int *quoted)
{
	*filename = NULL;
	*quoted = 0;
	if (cmd[*i + 1] && cmd[*i + 1]->type == 0)
	{
		*filename = cmd[*i + 1]->str;
		*quoted = (cmd[*i + 1]->quoted != 0);
	}
	else if (!cmd[*i + 1] && cmd[*i]->type != 2)
		return (-1);
	else if (cmd[*i + 1] && cmd[*i + 1]->type != 0)
		return (-1);
	return (0);
}

static int	handle_redirection_logic(t_token **cmd, char **envp, int *in_fd,
		int *out_fd, int *i)
{
	char	*filename;
	int		quoted;
	int		res;

	if (!(cmd[*i] && (cmd[*i]->type == 1 || cmd[*i]->type == 2
				|| cmd[*i]->type == 3 || cmd[*i]->type == 4)))
		return (0);
	if (fetch_filename(cmd, i, &filename, &quoted) == -1)
		return (-1);
	if (cmd[*i]->type == 1)
		res = apply_infile(filename, in_fd);
	else if (cmd[*i]->type == 2)
		res = apply_heredoc(filename, quoted, envp, in_fd);
	else if (cmd[*i]->type == 3)
		res = apply_outfile(filename, out_fd);
	else
		res = apply_append(filename, out_fd);
	if (res == -1)
		return (-1);
	free_token(cmd[*i]);
	cmd[*i] = NULL;
	if (cmd[*i + 1])
	{
		free_token(cmd[*i + 1]);
		cmd[*i + 1] = NULL;
	}
	if (filename)
		*i += 2;
	else
		*i += 1;
	return (1);
}

static int	process_token(t_token **cmd, t_token **clean, int *i, int *j,
		char **envp, int *in_fd, int *out_fd)
{
	int	res;

	res = handle_redirection_logic(cmd, envp, in_fd, out_fd, i);
	if (res == -1)
		return (-1);
	if (res == 0)
	{
		clean[*j] = cmd[*i];
		cmd[*i] = NULL;
		(*j)++;
		(*i)++;
	}
	return (0);
}

t_token	**handle_redirections(t_token **cmd, char **envp, int *in_fd,
		int *out_fd)
{
	int		i;
	int		j;
	int		count;
	t_token	**clean;

	i = 0;
	j = 0;
	count = count_tokens(cmd);
	clean = alloc_clean_array(count);
	if (!clean)
	{
		free_token_array(cmd, count);
		return (NULL);
	}
	while (i < count && cmd[i])
	{
		if (process_token(cmd, clean, &i, &j, envp, in_fd, out_fd) == -1)
		{
			free_token_array(cmd, count);
			free_token_array(clean, j);
			return (NULL);
		}
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
