#include "minishell.h"
#include "libft/libft.h"

/*
 * This file implements a very small pipeline executor. It tokenizes the
 * command string by the `|` character and chains the resulting segments using
 * UNIX pipes. Each segment is executed either as a builtin or via execve.
 */

/*
 * count_segments:
 *   Return the number of command segments in the pipeline array.
 */
static int  count_segments(char **segments)
{
    int i = 0;
    while (segments && segments[i])
        i++;
    return (i);
}

/*
 * execute_pipeline:
 *   Execute a sequence of commands separated by '|'. Each segment is
 *   launched in its own process with the appropriate stdin/stdout
 *   redirected through a pipe.
 */
void    execute_pipeline(char **envp, char **segments)
{
    int     num;    /* number of segments in the pipeline */
    pid_t   *pids;  /* array to store child process ids */
    int     in_fd;  /* read end of the previous pipe */
    int     fd[2];  /* file descriptors for the current pipe */
    int     i;

    num = count_segments(segments);             /* number of commands */
    pids = malloc(sizeof(pid_t) * num);         /* allocate pid array */
    if (!pids)
        return ;
    in_fd = 0;                                  /* start reading from stdin */
    i = -1;
    /* iterate over each command segment */
    while (++i < num)
    {
        /* tokenize current segment into command + args */
        char    **cmd = ft_tokenize(segments[i], ' ',  envp);
        if (!cmd || !cmd[0])
        {
            free_cmd(cmd);
            continue ;
        }
        /* create a pipe for all but the last command */
        if (i < num - 1 && pipe(fd) == -1)
        {
            perror("pipe");
            free_cmd(cmd);
            break ;
        }
        /* spawn child process */
        pids[i] = fork();
        if (pids[i] == 0)
        {
            /* child: hook up previous pipe to stdin */
            if (in_fd != 0)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            /* child: connect stdout to write end of current pipe */
            if (i < num - 1)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }
            /* execute builtin directly, otherwise try external command */
            if (is_builtin(cmd[0]))
                run_builtin(&envp, cmd);
            else
            {
                char *path = get_path(envp, cmd);
                if (path)
                {
                    execve(path, cmd, envp);
                    perror("execve");
                    free(path);
                }
                else
                    printf("Command not found: %s\n", cmd[0]);
            }
            free_cmd(cmd);
            _exit(1);
        }
        else if (pids[i] < 0)
        {
            /* fork failed */
            perror("fork");
            if (i < num - 1)
            {
                close(fd[0]);
                close(fd[1]);
            }
            free_cmd(cmd);
            break ;
        }
        /* parent: cleanup and prepare input for next command */
        if (in_fd != 0)
            close(in_fd);
        if (i < num - 1)
        {
            close(fd[1]);
            in_fd = fd[0];
        }
        else
            in_fd = 0;
        free_cmd(cmd);
    }
    /* close any remaining pipe end in parent */
    if (in_fd != 0)
        close(in_fd);
    /* wait for all child processes */
    while (--i >= 0)
        waitpid(pids[i], NULL, 0);
    free(pids);
}
