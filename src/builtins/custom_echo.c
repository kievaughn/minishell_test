/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:15:55 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 14:27:31 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static char *normalize_token(const char *str)
{
    char    *tmp;
    int     i;

    tmp = ft_strdup(str);
    if (!tmp)
        return (NULL);
    remove_quotes(tmp);
    i = 0;
    while (tmp[i])
    {
        tmp[i] = ft_tolower((unsigned char)tmp[i]);
        i++;
    }
    return (tmp);
}

static int skip_n_flags(t_token **arg, int start)
{
    int     i;
    int     j;
    char    *norm;

    i = start;
    while (arg[i])
    {
        norm = normalize_token(arg[i]->str);
        if (!norm || norm[0] != '-')
        {
            free(norm);
            break ;
        }
        j = 1;
        while (norm[j] == 'n')
            j++;
        if (norm[j] == '\0')
        {
            if (j == 2)
            {
                free(norm);
                int k = i + 1;
                while (arg[k])
                {
                    char *next = normalize_token(arg[k]->str);
                    if (next && next[0] == 'n' && next[1] == '\0' && arg[k]->quoted)
                    {
                        free(next);
                        k++;
                    }
                    else
                    {
                        free(next);
                        break ;
                    }
                }
                i = k;
                continue ;
            }
            free(norm);
            i++;
            continue ;
        }
        free(norm);
        break ;
    }
    return (i);
}

static int write_echo_arg(t_token *tok)
{
    char        *code;
    const char  *arg = tok->str;

    if (tok->quoted == 1)
    {
        write(1, arg, ft_strlen(arg));
        return (0);
    }
    if (ft_strncmp(arg, "$?", 3) == 0)
    {
        code = ft_itoa(g_exit_code);
        if (!code)
            return (1);
        write(1, code, ft_strlen(code));
        free(code);
    }
    else
        write(1, arg, ft_strlen(arg));
    return (0);
}

static int print_echo_args(t_token **arg, int start)
{
    int     i;

    i = start;
    while (arg[i])
    {
        if (write_echo_arg(arg[i]))
            return (1);
        if (arg[i + 1])
            write(1, " ", 1);
        i++;
    }
    return (0);
}

short int custom_echo(t_token **arg)
{
    int     i;
    int     flag_newline;

    i = 1;
    flag_newline = 1;
    i = skip_n_flags(arg, i);
    if (i > 1)
        flag_newline = 0;
    if (print_echo_args(arg, i))
        return (1);
    if (flag_newline)
        write(1, "\n", 1);
    return (0);
}

