/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:29 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/16 16:00:15 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <limits.h>

void sigint_handler(int signum)
{
    (void)signum;

    write(STDOUT_FILENO, "\n", 1);

    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    char *line;
    char **env;
    
    env = copy_envp(envp);
    signal(SIGINT, sigint_handler);
    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;

        if (*line)
            add_history(line);

        process_command(env, line);
        free(line);
    }
    return (0);
}

