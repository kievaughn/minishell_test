/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:55 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/18 19:40:42 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

int skip_n_flags(char **arg, int start)
{
    int i;
    int j;
    int all_n;
    
    i = start;
    while (arg[i] && ft_strncmp(arg[i], "-n", 2) == 0)
    {
        j = 1;
        all_n = 1;
        while (arg[i][j])
        {
            if (arg[i][j] != 'n')
            {
                all_n = 0;
                break;
            }
            j++;
        }
        if (!all_n)
            break;
        i++;
    }
    return i;
}

short int custom_echo(char **arg)
{
    int i = 1;
    int flag_newline = 1;
    char *code;

    i = skip_n_flags(arg, i);
    if (i > 1)
        flag_newline = 0;

    while (arg[i])
    {
        if (ft_strncmp(arg[i], "$?", 3) == 0)
        {
            code = ft_itoa(last_exit_code);
            if (!code)
                return (1);
            write(1, code, ft_strlen(code));
            free(code);
        }
        else
        {
            write(1, arg[i], ft_strlen(arg[i]));
        }

        if (arg[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (flag_newline)
        write(1, "\n", 1);

    return (0);
}
