/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:29 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/30 13:42:13 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int		g_exit_code = 0;

void	sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**env;

	(void)argc;
	(void)argv;
	env = copy_envp(envp);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		process_command(&env, line);
		free(line);
	}
	free_cmd(env);
	return (0);
}
