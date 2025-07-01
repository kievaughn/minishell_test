#include "minishell.h"
#include "libft/libft.h"

static int  count_segments(char **segments)
{
    int i = 0;
    while (segments && segments[i])
        i++;
    return (i);
}

void    execute_pipeline(char **envp, char **segments)
{
    int     num;
    pid_t   *pids;
    int     in_fd;
    int     fd[2];
    int     i;

    num = count_segments(segments);
    pids = malloc(sizeof(pid_t) * num);
    if (!pids)
        return ;
    in_fd = 0;
    i = -1;
    while (++i < num)
    {
        char    **cmd = ft_tokenize(segments[i], ' ',  envp);
        if (!cmd || !cmd[0])
        {
            free_cmd(cmd);
            continue ;
        }
        if (i < num - 1 && pipe(fd) == -1)
        {
            perror("pipe");
            free_cmd(cmd);
            if (in_fd != 0)
                close(in_fd);
            break ;
        }
        pids[i] = fork();
        if (pids[i] == 0)
        {
            if (in_fd != 0)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i < num - 1)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }
            if (is_builtin(cmd[0]))
            {
                run_builtin(&envp, cmd);
                free_cmd(cmd);
                exit(last_exit_code);
            }
            else
            {
                char *path = get_path(envp, cmd);
                if (path)
                {
                    execve(path, cmd, envp);
                    perror("execve");
                    free(path);
                    last_exit_code = 126;
                }
                else
                {
                    fprintf(stderr, "Command not found: %s\n", cmd[0]);
                    last_exit_code = 127;
                }
                free_cmd(cmd);
                exit(last_exit_code);
            }
        }
        else if (pids[i] < 0)
        {
            perror("fork");
            if (i < num - 1)
            {
                close(fd[0]);
                close(fd[1]);
            }
            if (in_fd != 0)
                close(in_fd);
            free_cmd(cmd);
            break ;
        }
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
    if (in_fd != 0)
        close(in_fd);
    {
        int status;
        int j = 0;
        while (j < num)
        {
            if (waitpid(pids[j], &status, 0) == -1)
                perror("waitpid");
            if (j == num - 1)
            {
                if (WIFEXITED(status))
                    last_exit_code = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    last_exit_code = 128 + WTERMSIG(status);
                else
                    last_exit_code = 1;
            }
            j++;
        }
    }
    free(pids);
}
