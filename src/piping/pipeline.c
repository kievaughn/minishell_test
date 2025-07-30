#include "minishell.h"

static void     child_process(
        char **envp, char **cmd, int in_fd, int *fd, int last
)
{
        int     redir_in;
        int     redir_out;

        redir_in = STDIN_FILENO;
        redir_out = STDOUT_FILENO;
        cmd = handle_redirections(cmd, &redir_in, &redir_out);
		if (cmd == NULL)
    		exit(1);
        if (redir_in != STDIN_FILENO)
        {
			if (redir_in < 0)
			{
				perror("Input redirection");
				exit(1);
			}
                dup2(redir_in, STDIN_FILENO);
                close(redir_in);
        }
        else if (in_fd != STDIN_FILENO)
        {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
        }
        if (!last && redir_out == STDOUT_FILENO)
                dup2(fd[1], STDOUT_FILENO);
        else if (redir_out != STDOUT_FILENO)
		{
			if (redir_out < 0)
			{
				perror("Output redirection");
				exit(1);
			}
			dup2(redir_out, STDOUT_FILENO);
		}
        if (!last)
        {
                close(fd[0]);
                close(fd[1]);
        }
        if (redir_out != STDOUT_FILENO)
                close(redir_out);
        execute_cmd(envp, cmd);
}


static int	pipeline_step(
	char **envp, char **segments, pid_t *pids,
	int *in_fd, int *fd, int i, int num
)
{
	char	**cmd;

	if (i < num - 1 && pipe(fd) == -1)
	{
		perror("pipe");
		return (0);
	}

	cmd = tokenize_command(segments[i], ' ', envp);
	if (!cmd || !cmd[0])
	{
		free_cmd(cmd);
		if (i < num - 1)
			close_pipe(fd);
		return (1);
	}
	pids[i] = fork();
	if (pids[i] == 0)
		child_process(envp, cmd, *in_fd, fd, i == num - 1);
	free_cmd(cmd);
	parent_cleanup(in_fd, fd, i, num);
	return (1);
}

static void	pipeline_loop(
	char **envp, char **segments, pid_t *pids, int num
)
{
	int	in_fd;
	int	fd[2];
	int	i;

	in_fd = STDIN_FILENO;
	i = 0;
	while (i < num)
	{
		if (!pipeline_step(envp, segments, pids, &in_fd, fd, i, num))
			break ;
		i++;
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
}

void	execute_pipeline(char **envp, char **segments)
{
	int		num;
	pid_t	*pids;

	num = count_strings(segments);
	pids = malloc(sizeof(pid_t) * num);
	if (!pids)
		return ;
	pipeline_loop(envp, segments, pids, num);
	wait_for_all(pids, num);
	free(pids);
}
