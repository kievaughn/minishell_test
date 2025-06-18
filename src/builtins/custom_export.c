/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:02:23 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/18 19:41:18 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

int is_valid_name(const char *name)
{
    int i;

    if (!name || !name[0])
        return (0);

    if (!ft_isalpha(name[0]) && name[0] != '_')
        return (0);

    i = 1;
    while (name[i] && name[i] != '=')
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
        i++;
    }

    return (1);
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

short int check_update_env(char ***env, char **args)
{
    int arg_i;
    int i;
    int size;
    int found;

    arg_i = 1;
    while (args[arg_i])
    {
        if(!is_valid_name(args[arg_i]))
        {
            fprintf(stderr, "export: not an identifier: %s \n", args[arg_i]);
            arg_i++;
            continue;
        }
        size = 0;
        while (args[arg_i][size] && args[arg_i][size] != '=')
            size++;
        i = 0;
        found = 0;
        while ((*env)[i])
        {
        if (ft_strncmp((*env)[i], args[arg_i], size) == 0 &&
            ((*env)[i][size] == '=' || (*env)[i][size] == '\0'))
            {
                if(args[arg_i][size] == '=')
                {
                    free((*env)[i]);
                    (*env)[i] = ft_strdup(args[arg_i]);
                }
                    found = 1;
                    break;
            }
            i++;
        }
        if (!found)
        {
            if(env_add(env, args[arg_i]) == -1)
                return (-1);
        }
        arg_i++;
    }
    return (0);
}

short int custom_export(char ***env, char **args)
{
    int size;
    int *index;
    int i;

    size = env_size(*env);
    index = malloc(sizeof(int) * size);
    if (!index)
        return (1);
    i = -1;
    while (++i < size)
        index[i] = i;
        
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
