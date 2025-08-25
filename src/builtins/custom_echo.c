/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:55 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:56:08 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static int	skip_n_flags(t_token **arg, int start)
{
	int	i;
	int	j;
	int	all_n;

	i = start;
	while (arg[i] && ft_strncmp(arg[i]->str, "-n", 2) == 0)
	{
		j = 1;
		all_n = 1;
		while (arg[i]->str[j])
		{
			if (arg[i]->str[j] != 'n')
			{
				all_n = 0;
				break ;
			}
			j++;
		}
		if (!all_n)
			break ;
		i++;
	}
	return (i);
}

static int	write_echo_arg(const char *arg)
{
	char	*code;

	if (ft_strncmp(arg, "$?", 3) == 0)
	{
		code = ft_itoa(g_exit_code);
		if (!code)
			return (1);
		write(1, code, ft_strlen(code));
		free(code);
	}
	else
		write(1, arg, ft_strlen(arg));
	return (0);
}

static int	print_echo_args(t_token **arg, int start)
{
	int	i;

	i = start;
	while (arg[i])
	{
		if (write_echo_arg(arg[i]->str))
			return (1);
		if (arg[i + 1])
			write(1, " ", 1);
		i++;
	}
	return (0);
}

short int	custom_echo(t_token **arg)
{
	int	i;
	int	flag_newline;

	i = 1;
	flag_newline = 1;
	i = skip_n_flags(arg, i);
	if (i > 1)
		flag_newline = 0;
	if (print_echo_args(arg, i))
		return (1);
	if (flag_newline)
		write(1, "\n", 1);
	return (0);
}

