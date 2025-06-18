/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:51:10 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/18 18:04:59 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

short int search_var(const char *env_var, const char *var)
{
    int i;

    i = 0;
    while (env_var[i] && env_var[i] != '=' && var[i])
    {
        if (env_var[i] != var[i])
            return 0;
        i++;
    }

    return (env_var[i] == '=' && var[i] == '\0');
}

void remove_var(char ***envp, const char *var)
{
    int i = 0;

    while ((*envp)[i])
    {
        if (search_var((*envp)[i], var))
        {
            free((*envp)[i]);
            int pos = i;
            while ((*envp)[pos + 1])
            {
                (*envp)[pos] = (*envp)[pos + 1];
                pos++;
            }
            (*envp)[pos] = NULL;
        }
        else
        {
            i++;
        }
    }
}

short int custom_unset(char ***envp, char **args)
{
    int i = 1;

    while (args[i])
    {
        remove_var(envp, args[i]);
        i++;
    }
    return 0;
}
