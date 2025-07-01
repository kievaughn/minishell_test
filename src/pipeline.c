#include "minishell.h"
#include "libft/libft.h"

static int	count_segments(char **segments)
{
	int	i;

	i = 0;
	while (segments && segments[i])
		i++;
	return (i);
}

static void	execute_cmd(char **envp, char **cmd)
{
	char	*path;

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

static void	child_process(
	char **envp, char **cmd, int in_fd, int *fd, int last
)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (!last)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	execute_cmd(envp, cmd);
}

static void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

static void	parent_cleanup(
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

static void	wait_for_all(pid_t *pids, int count)
{
	int	i;
	int	status;

	i = 0;
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
	cmd = ft_tokenize(segments[i], ' ', envp);
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

	num = count_segments(segments);
	pids = malloc(sizeof(pid_t) * num);
	if (!pids)
		return ;
	pipeline_loop(envp, segments, pids, num);
	wait_for_all(pids, num);
	free(pids);
}
