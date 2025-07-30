/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/30 13:45:21 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

short int custom_exit(char **args)
{
    long long value;

    if (args[1] && args[2])
    {
        fprintf(stderr, "exit: too many arguments\n");
        g_exit_code = 1;
        return (1);
    }
    printf("exit\n");
    if (args[1])
    {
        if (!ft_atoany(args[1], &value))
        {
            fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
            exit(2);
        }
        exit((unsigned char)value);
    }

    exit(g_exit_code);
}
