/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:18:47 by kbrandon          #+#    #+#             */
/*   Updated: 2025/08/05 17:43:49 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static int	is_exit_code(char *s, int i)
{
	return (s[i] == '$' && s[i + 1] == '?');
}

static int	is_var(char *s, int i)
{
	return (s[i] == '$' && ft_isalnum((unsigned char)s[i + 1]));
}

static char	*append_remain(char *res, char *s, int start)
{
	return (ft_strcatrealloc(res, s + start));
}

char	*build_expanded_str(char *s, char **envp)
{
	int		i;
	int		start;
	char	*res;

	i = 0;
	start = 0;
	res = NULL;
	while (s[i])
	{
		if (is_exit_code(s, i))
			res = expand_exit_code(res, &i, s);
		else if (is_var(s, i))
		{
			res = append_literal(res, s, start, i);
			if (!res)
				return (NULL);
			res = append_expanded_var(res, s, &i, envp);
			if (!res)
				return (NULL);
		}
		else
			i++;
		start = i;
	}
	return (append_remain(res, s, start));
}

/*
char	*build_expanded_str(char *s, char **envp)
{
	int		i;
	int		start;
	char	*res;

	i = 0;
	start = 0;
	res = NULL;
	while (s[i])
	{
		if (is_exit_code(s, i))
			res = expand_exit_code(res, &i, s);
		else if (is_var(s, i))
			res = expand_var(res, &i, s, envp);
		else
			i++;
		start = i;
	}
	return (append_remain(res, s, start));
}*/
