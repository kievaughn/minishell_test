/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:29 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/28 00:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include "token_utils.h"

int             g_exit_code = 0;

void    sigint_handler(int signum)
{
        (void)signum;
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
}

static char     *read_input(const char *prompt, int istty)
{
        char    *line;
        char    *tmp;

        if (istty)
                line = readline(prompt);
        else
        {
                tmp = get_next_line(fileno(stdin));
                if (!tmp)
                        return (NULL);
                line = ft_strtrim(tmp, "\n");
                free(tmp);
        }
        return (line);
}

int     main(int argc, char **argv, char **envp)
{
        char    *line;
        char    *more;
        char    *tmp;
        char    **env;
        int             istty;

        (void)argc;
        (void)argv;
        env = copy_envp(envp);
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
        istty = isatty(fileno(stdin));
        while (1)
        {
                line = read_input("minishell$ ", istty);
                if (!line)
                        break ;
                while (has_unclosed_quotes(line))
                {
                        more = read_input("> ", istty);
                        if (!more)
                        {
                                error_msg("minishell", "unexpected EOF while looking for matching quote");
                                g_exit_code = 2;
                                free(line);
                                line = NULL;
                                break ;
                        }
                        tmp = ft_strjoin(line, "\n");
                        free(line);
                        line = ft_strjoin(tmp, more);
                        free(tmp);
                        free(more);
                }
                if (!line)
                        break ;
                if (*line)
                        add_history(line);
                process_command(&env, line);
                free(line);
        }
        free_cmd(env);
        return (g_exit_code);
}

