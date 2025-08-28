/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:22:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 12:55:59 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

int token_array_len(t_token **tokens)
{
    int i = 0;

    if (!tokens)
        return 0;
    while (tokens[i])
        i++;
    return i;
}

char **prepare_argv_from_tokens(t_token **tokens)
{
    int count = token_array_len(tokens);
    char **argv = malloc(sizeof(char *) * (count + 1));
    if (!argv)
        return (NULL);

    int i = 0;
    while (i < count)
    {
        argv[i] = ft_strdup(tokens[i]->str);
        if (!argv[i])
            return (free_cmd(argv), NULL);
        /*
         * Quotes that affected tokenisation have already been removed during
         * parsing.  Any remaining quotes in the token are meant to be literal
         * characters (e.g. the command `echo '"$USER"'` should pass the
         * string "\"$USER\"" to echo).  Stripping quotes here would modify
         * the intended argument and lead to behaviour that differs from Bash.
         */
        i++;
    }
    argv[count] = NULL;
    return argv;
}

static void child_exec(char *path, char **argv, char **envp)
{
    execve(path, argv, envp);
    error_perror("execve");
    free_cmd(argv);
    _exit(127);
}

static int parent_wait(pid_t pid, char **argv)
{
    int status;

    free_cmd(argv);
    if (waitpid(pid, &status, 0) == -1)
    {
        error_perror("waitpid");
        return (1);
    }
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    else
        return (1);
}

int execute_command(char *path, t_token **tokens, char **envp)
{
    pid_t   pid;
    char  **argv;

    argv = prepare_argv_from_tokens(tokens);
    if (!argv)
        return (1);

    pid = fork();
    if (pid == 0)
    {
        child_exec(path, argv, envp);
        return (0);
    }
    else if (pid > 0)
        return (parent_wait(pid, argv));
    else
    {
        error_perror("fork");
        free_cmd(argv);
        return (1);
    }
}

