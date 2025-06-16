/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:02:23 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/16 17:21:45 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

int env_size(char **env)
{
    int size;
    
    size = 0;
    while (env[size])
        size++;
        
    return (size);
}

void sort_index(char **env, int *index, int size)
{
    int i;
    int j;
    int aux;
    int len;

    i = -1;
    while (++i < size - 1)
    {
        j = i;
        while (++j < size)
        {
            if (ft_strlen(env[index[i]]) > ft_strlen(env[index[j]]))
                len = ft_strlen(env[index[i]]);
            else
                len = ft_strlen(env[index[j]]);
            if (ft_strncmp(env[index[i]], env[index[j]], len) > 0)
            {
                aux = index[i];
                index[i] = index[j];
                index[j] = aux;
            }
        }
    }
}

void print_env(char **env, int *index, int size)
{
    int i;
    int j;

    i = -1;
    while (++i < size)
    {
        j = -1;
        write(1, "declare -x ", 11);
        while (env[index[i]][++j] && env[index[i]][j] != '=')
            write(1, &env[index[i]][j], 1);

        if (env[index[i]][j] == '=')
        {
            write(1, "=\"", 2);
            j++;
            while (env[index[i]][j])
            {
                write(1, &env[index[i]][j], 1);
                j++;
            }
            write(1, "\"", 1);
        }
        write(1, "\n", 1);
    }
}

void custom_export(char **env)
{
    int size;
    int *index;
    int i;

    size = env_size(env);
    index = malloc(sizeof(int) * size);
    i = 0;
    if (!index)
        return;
    while (i < size)
    {
        index[i] = i;
        i++;
    }
    sort_index(env, index, size);
    print_env(env, index, size);

    free(index);
}
