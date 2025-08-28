/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:56:49 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

short int	custom_exit(t_token **args)
{
	long long	value;

	fprintf(stderr, "exit\n");
	if (args[1] && args[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		g_exit_code = 1;
		return (1);
	}
	if (args[1])
	{
		if (!ft_atoany(args[1]->str, &value))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n",
				args[1]->str);
			g_exit_code = 2;
			exit(g_exit_code);
		}
		g_exit_code = (unsigned char)value;
		exit(g_exit_code);
	}
	exit(g_exit_code);
}
