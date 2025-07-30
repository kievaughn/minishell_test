#include "minishell.h"
#include "../libft/libft.h"

static void     write_heredoc(int fd, const char *delim)
{
    char    *line;

    while (1)
    {
            line = readline("> ");
            if (!line || ft_strcmp(line, delim) == 0)
            {
                    free(line);
                    break ;
            }
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
    }
}

static int      handle_heredoc(const char *delim, int *in_fd)
{
    int             pipefd[2];

    if (pipe(pipefd) == -1)
    {
            perror("pipe");
            g_exit_code = 1;
            return (-1);
    }
    write_heredoc(pipefd[1], delim);
    close(pipefd[1]);
    if (*in_fd != STDIN_FILENO)
            close(*in_fd);
    *in_fd = pipefd[0];
    return (0);
}


static int      apply_in_redir(char **cmd, int *i, int *in_fd)
{
    if (!ft_strcmp(cmd[*i], "<") && cmd[*i + 1])
    {
            if (open_infile(cmd[*i + 1], in_fd) == -1)
                    return (-1);
            *i += 2;
            return (1);
    }
    if (!ft_strcmp(cmd[*i], "<<") && cmd[*i + 1])
    {
            if (handle_heredoc(cmd[*i + 1], in_fd) == -1)
                    return (-1);
            *i += 2;
            return (1);
    }
    return (0);
}

static int      apply_redir(char **cmd, int *i, int *in_fd, int *out_fd)
{
    int     ret;

    ret = apply_in_redir(cmd, i, in_fd);
    if (ret)
            return (ret);
    if (!ft_strcmp(cmd[*i], ">") && cmd[*i + 1])
    {
            if (open_outfile(cmd[*i + 1], out_fd) == -1)
                    return (-1);
            *i += 2;
            return (1);
    }
    if (!ft_strcmp(cmd[*i], ">>") && cmd[*i + 1])
    {
            if (open_appendfile(cmd[*i + 1], out_fd) == -1)
                    return (-1);
            *i += 2;
            return (1);
    }
    return (0);
}


char **handle_redirections(char **cmd, int *in_fd, int *out_fd)
{
    int     i;
    int     j;
    char    **clean;
    int     ret;
    i = 0; j = 0;
    clean = malloc(sizeof(char *) * (count_strings(cmd) + 1));
    if (!clean)
            return (NULL);
    while (cmd[i])
    {
            ret = apply_redir(cmd, &i, in_fd, out_fd);
            if (ret == -1)
            {
                    free(clean);
                    return (NULL);
            }
            if (ret == 0)
                    clean[j++] = cmd[i++];
    }
    clean[j] = NULL;
    free(cmd);
    return (clean);
}

