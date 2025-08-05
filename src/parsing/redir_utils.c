/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:18:53 by kbrandon          #+#    #+#             */
/*   Updated: 2025/08/05 16:26:35 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static void	handle_redir_count(char *tok, int *i, int *start, int *count)
{
	if (*i - *start > 0)
		(*count)++;
	if (tok[*i] == '>' && tok[*i + 1] == '>')
	{
		(*count)++;
		*i += 2;
	}
	else
	{
		(*count)++;
		(*i)++;
	}
	*start = *i;
}

static int	part_count(char *tok)
{
	int	i;
	int	start;
	int	count;

	i = 0;
	start = 0;
	count = 0;
	while (tok[i])
	{
		if (tok[i] == '>' || tok[i] == '<')
			handle_redir_count(tok, &i, &start, &count);
		else
			i++;
	}
	if (i - start > 0)
		count++;
	return (count);
}

int	total_parts(char **arr)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (arr && arr[i])
	{
		total += part_count(arr[i]);
		i++;
	}
	return (total);
}
