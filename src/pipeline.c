#include "minishell.h"
#include "libft/libft.h"

static int count_segments(char **segments)
{
    int i;

    i = 0;
    while (segments && segments[i])
        i++;
    return (i);
}

static void redirect_fds(int fd_in, int fd_out, int last)
{
    if (fd_in != STDIN_FILENO)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (!last)
    {
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
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

static void run_child(char **envp, char **cmd, int fd_in, int fd_out, int last)
{
    redirect_fds(fd_in, fd_out, last);
    execute_cmd(envp, cmd);
}

static void wait_children(pid_t *pids, int num)
{
    int status;
    int i;

    i = 0;
    while (i < num)
    {
        if (waitpid(pids[i], &status, 0) == -1)
            perror("waitpid");
        if (i == num - 1)
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

void execute_pipeline(char **envp, char **segments)
{
    pid_t *pids;
    int fd[2];
    int in_fd;
    int num;
    int i;
    char **cmd;

    num = count_segments(segments);
    pids = malloc(sizeof(pid_t) * num);
    if (!pids)
        return ;
    in_fd = STDIN_FILENO;
    i = 0;
    while (i < num)
    {
        if (i < num - 1 && pipe(fd) == -1)
        {
            perror("pipe");
            break ;
        }
        cmd = ft_tokenize(segments[i], ' ', envp);
        if (!cmd || !cmd[0])
        {
            free_cmd(cmd);
            i++;
            continue ;
        }
        pids[i] = fork();
        if (pids[i] == 0)
            run_child(envp, cmd, in_fd, fd[1], i == num - 1);
        else if (pids[i] < 0)
        {
            perror("fork");
            free_cmd(cmd);
            break ;
        }
        free_cmd(cmd);
        if (in_fd != STDIN_FILENO)
            close(in_fd);
        if (i < num - 1)
        {
            close(fd[1]);
            in_fd = fd[0];
        }
        i++;
    }
    if (in_fd != STDIN_FILENO)
        close(in_fd);
    wait_children(pids, i);
    free(pids);
}

