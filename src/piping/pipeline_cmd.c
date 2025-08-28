/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"
#include <errno.h>

static int  exit_code_from_errno(void)
{
    if (errno == ENOENT)
        return (127);
    return (126);
}

static char **tokens_to_argv(t_token **cmd)
{
    int count = 0;
    char **argv;
    int i;

    while (cmd && cmd[count])
        count++;
    argv = malloc((count + 1) * sizeof(char *));
    if (!argv)
        return (NULL);
    i = 0;
    while (i < count)
    {
        argv[i] = ft_strdup(cmd[i]->str);
        i++;
    }
    argv[count] = NULL;
    return (argv);
}

void    execute_cmd(char **envp, t_token **cmd)
{
    char *path;
    char **argv;

    argv = tokens_to_argv(cmd);
    if (!argv || !argv[0])
    {
        free_cmd(argv);
        free_tokens(cmd);
        exit(0);
    }
    if (is_builtin(argv[0]))
    {
        run_builtin(&envp, cmd);
        free_cmd(argv);
        free_tokens(cmd);
        exit(g_exit_code);
    }
    if (ft_strchr(argv[0], '/'))
    {
        if (access(argv[0], F_OK) != 0)
        {
            error_perror(argv[0]);
            g_exit_code = 127;
        }
        else if (is_folder(argv[0]))
        {
            error_msg(argv[0], "Is a directory");
            g_exit_code = 126;
        }
        else if (access(argv[0], X_OK) != 0)
        {
            error_perror(argv[0]);
            g_exit_code = 126;
        }
        else
        {
            execve(argv[0], argv, envp);
            error_perror(argv[0]);
            g_exit_code = exit_code_from_errno();
        }
    }
    else
    {
        path = get_path(envp, argv);
        if (path)
        {
            execve(path, argv, envp);
            error_perror(argv[0]);
            free(path);
            g_exit_code = exit_code_from_errno();
        }
        else
        {
            error_msg(argv[0], "command not found");
            g_exit_code = 127;
        }
    }
    free_cmd(argv);
    free_tokens(cmd);
    exit(g_exit_code);
}

