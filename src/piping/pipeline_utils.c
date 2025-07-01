#include "minishell.h"

void execute_cmd(char **envp, char **cmd)
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

void close_pipe(int *fd)
{
    close(fd[0]);
    close(fd[1]);
}

void parent_cleanup(
    int *in_fd, int *fd, int i, int num
)
{
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    if (i < num - 1)
    {
        close(fd[1]);
        *in_fd = fd[0];
    }
}

void wait_for_all(pid_t *pids, int count)
{
    int i = 0;
    int status;
    while (i < count)
    {
        if (waitpid(pids[i], &status, 0) == -1)
            perror("waitpid");
        if (i == count - 1)
        {
            if (WIFEXITED(status))
                last_exit_code = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_exit_code = 128 + WTERMSIG(status);
            else
                last_exit_code = 1;
        }
        i++;
    }
}