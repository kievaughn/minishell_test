/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:41:47 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:56:30 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

short int	custom_env(char **envp, t_token **args)
{
	int	i;

	if (args[1])
	{
		fprintf(stderr, "env: too many arguments\n");
		g_exit_code = 1;
		return (1);
	}
	if (!envp)
		return (1);
	i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

