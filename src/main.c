/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:29 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 10:07:27 by dimendon         ###   ########.fr       */
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
		//line = readline("minishell$ ");
		if (isatty(fileno(stdin))) //Snippet for tester
			line = readline("minishell$ ");
		else
		{
			char *getline;
			getline = get_next_line(fileno(stdin));
			if(!getline)
				break;
			line = ft_strtrim(getline, "\n");
			free(getline);
		}
		if (line == NULL)
			break ;
		char *temp;
		temp = ft_strdup(line);
		free(line);
		line = temp;
		if (*line)
			add_history(line);
		process_command(&env, line);
		free(line);
	}
	free_cmd(env);
	return (g_exit_code);
}
