/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/13 18:20:43 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

short int custom_exit(char **args)
{
    long long value;

    printf("exit\n");

    if (!args || !args[1])
        exit(0);

    if (!ft_atoany(args[1], &value))
    {
        printf("exit: %s: numeric argument required\n", args[1]);
        exit(255);
    }
    if (args[2])
    {
        printf("exit: too many arguments\n");
        return (1);
    }
    exit((unsigned char)(value % 256));
}
