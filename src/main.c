/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kievaughn <kievaughn@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:29 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/18 19:03:59 by dimendon         ###   ########.fr       */
/*   Updated: 2025/06/18 18:33:38 by kievaughn        ###   ########.fr       */
/*   Updated: 2025/06/18 17:42:47 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "libft/libft.h"
#include <limits.h>

int last_exit_code = 0;

void sigint_handler(int signum)
{
    (void)signum;

    write(STDOUT_FILENO, "\n", 1);

    rl_on_new_line();
//    rl_replace_line("", 0);
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

        process_command(&env, line);
        free(line);
    }
    return (0);
}

