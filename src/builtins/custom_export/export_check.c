/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:43:08 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:54:29 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static short int	check_update_env(char ***env, t_token **args)
{
	int			arg_i;
	short int	had_error;

	arg_i = 1;
	had_error = 0;
	while (args[arg_i])
	{
		if (!is_valid_name(args[arg_i]->str))
		{
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n",
				args[arg_i]->str);
			had_error = -1;
			arg_i++;
			continue ;
		}
		if (update_or_add_env(env, args[arg_i]->str) == -1)
			return (-1);
		arg_i++;
	}
	return (had_error);
}

short int	custom_export(char ***env, t_token **args)
{
	int	size;
	int	*index;

	size = env_size(*env);
	index = malloc(sizeof(int) * size);
	if (!index)
		return (1);
	init_export_index(index, size);
	sort_index(*env, index, size);
	if (!args[1])
		print_env(*env, index, size);
	else
	{
		if (check_update_env(env, args) == -1)
		{
			free(index);
			return (1);
		}
	}
	free(index);
	return (0);
}

