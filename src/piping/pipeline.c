#include "minishell.h"

typedef struct s_step
{
    int     in_fd;
    int     out_fd;
    int     pipe_fd[2];
    char  **cmd;
}   t_step;

static void child_process(t_step *st, char ***envp, int last)
{
    int save_in;
    int save_out;

    save_in  = 0;
    save_out = 0;
    setup_redirections(st->in_fd, st->out_fd, &save_in, &save_out);
    if (!last && st->out_fd == STDOUT_FILENO)
        dup2(st->pipe_fd[1], STDOUT_FILENO);
    if (!last)
    {
        close(st->pipe_fd[0]);
        close(st->pipe_fd[1]);
    }
    close(save_in);
    close(save_out);
    execute_cmd(*envp, st->cmd);
}

static int pipeline_step(
    t_step   *st,
    char    ***envp,
    pid_t    *pid,
    int       last
)
{
    if (!last && pipe(st->pipe_fd) == -1)
    {
        perror("pipe");
        return (0);
    }
    st->cmd = prepare_command(st->cmd[0], &st->in_fd, &st->out_fd, envp);
    if (!st->cmd)
    {
        if (!last)
            close_pipe(st->pipe_fd);
        if (st->in_fd != STDIN_FILENO)
            close(st->in_fd);
        if (st->out_fd != STDOUT_FILENO)
            close(st->out_fd);
        pid[0] = -1;
        return (1);
    }
    pid[0] = fork();
    if (pid[0] == 0)
        child_process(st, envp, last);
    parent_cleanup(&st->in_fd, st->pipe_fd, 0, last ? 1 : 2);
    if (st->out_fd != STDOUT_FILENO)
        close(st->out_fd);
    free_cmd(st->cmd);
    return (1);
}

static void pipeline_loop(
    char   ***envp,
    char    **segments,
    pid_t   *pids,
    int       num
)
{
    t_step   st;
    int      i;

    st.in_fd = STDIN_FILENO;
    i        = 0;
    while (i < num)
    {
        st.cmd      = &segments[i];
        st.out_fd   = STDOUT_FILENO;
        st.pipe_fd[0] = -1;
        st.pipe_fd[1] = -1;
        if (!pipeline_step(&st, envp, &pids[i], i == num - 1))
            break;
        i++;
    }
    if (st.in_fd != STDIN_FILENO)
        close(st.in_fd);
}

void execute_pipeline(char **envp, char **segments)
{
    int    num;
    pid_t *pids;

    num  = count_strings(segments);
    pids = malloc(sizeof(pid_t) * num);
    if (!pids)
        return;
    pipeline_loop(&envp, segments, pids, num);
    wait_for_all(pids, num);
    free(pids);
}
