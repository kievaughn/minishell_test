/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:51:10 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 11:57:17 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static void remove_var(char ***envp, const char *var)
{
    int i = 0;
    int pos;

    while ((*envp)[i])
    {
        if (ft_strncmp((*envp)[i], var, ft_strlen(var)) == 0
            && (*envp)[i][ft_strlen(var)] == '=')
        {
            free((*envp)[i]);
            pos = i;
            while ((*envp)[pos + 1])
            {
                (*envp)[pos] = (*envp)[pos + 1];
                pos++;
            }
            (*envp)[pos] = NULL;
        }
        else
            i++;
    }
}

short int	custom_unset(char ***envp, t_token **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_var(envp, args[i]->str);
		i++;
	}
	g_exit_code = 0;
	return (0);
}

