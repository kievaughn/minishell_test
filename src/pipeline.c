#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int count_segments(char **segments)
{
    int i = 0;
    while (segments && segments[i])
        i++;
    return i;
}

static void execute_cmd(char **envp, char **cmd)
{
    char *path;
    if (is_builtin(cmd[0]))
    {
        run_builtin(&envp, cmd);
        free_cmd(cmd);
        exit(last_exit_code);
    }
    path = get_path(envp, cmd);
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

void execute_pipeline(char **envp, char **segments)
{
    int num = count_segments(segments);
    pid_t *pids = malloc(sizeof(pid_t) * num);
    int in_fd = STDIN_FILENO;
    int fd[2];
    int i;

    if (!pids)
        return;

    for (i = 0; i < num; ++i)
    {
        if (i < num - 1 && pipe(fd) == -1)
        {
            perror("pipe");
            break;
        }

        char **cmd = ft_tokenize(segments[i], ' ', envp);
        if (!cmd || !cmd[0])
        {
            free_cmd(cmd);
            if (i < num - 1)
            {
                close(fd[0]);
                close(fd[1]);
            }
            continue;
        }

        pids[i] = fork();
        if (pids[i] == 0)
        {
            // Child process
            if (in_fd != STDIN_FILENO)
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
            execute_cmd(envp, cmd); // does not return
        }
        // Parent process
        free_cmd(cmd);
        if (in_fd != STDIN_FILENO)
            close(in_fd);
        if (i < num - 1)
        {
            close(fd[1]);
            in_fd = fd[0];
        }
    }
    if (in_fd != STDIN_FILENO)
        close(in_fd);

    // Wait for all children and set exit code from the last
    int status, last_status = 1;
    for (int j = 0; j < i; ++j)
    {
        if (waitpid(pids[j], &status, 0) == -1)
            perror("waitpid");
        if (j == i - 1)
        {
            if (WIFEXITED(status))
                last_exit_code = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_exit_code = 128 + WTERMSIG(status);
            else
                last_exit_code = 1;
        }
    }
    free(pids);
}
