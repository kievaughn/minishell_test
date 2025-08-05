#include "minishell.h"
#include "../libft/libft.h"
#include <errno.h> 

int open_infile(char *file, int *in_fd)
{
    int fd;

    fd = 0;
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);

    fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror(file);
        if (errno == EACCES)
            g_exit_code = 126;
        else if (errno == ENOENT)
            g_exit_code = 127;
        else
            g_exit_code = 1;
        return -1;
    }
    *in_fd = fd;
    return 0;
}

int open_outfile(char *file, int *out_fd)
{
    int fd;
    fd = 0;
    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);

    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror(file);
        if (errno == EACCES)
            g_exit_code = 126;
        else
            g_exit_code = 1;
        return -1;
    }

    *out_fd = fd;
    return 0;
}

int open_appendfile(char *file, int *out_fd)
{
    int fd;
    fd = 0;
    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);

    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror(file);
        if (errno == EACCES)
            g_exit_code = 126;
        else
            g_exit_code = 1;
        return -1;
    }

    *out_fd = fd;
    return 0;
}

static int open_heredoc(const char *delim)
{
    int pipefd[2];
    char *line;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        g_exit_code = 1;
        return -1;
    }

    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delim) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    close(pipefd[1]);
    return pipefd[0];
}

static int handle_heredoc(const char *delim, int *in_fd)
{
    int h = open_heredoc(delim);
    if (h < 0)
        return -1;

    if (*in_fd != STDIN_FILENO)
        close(*in_fd);

    *in_fd = h;
    return 0;
}

char **handle_redirections(char **cmd, int *in_fd, int *out_fd)
{
    int     i = 0;
    int     j = 0;
    int     cnt = count_strings(cmd);
    char  **clean = malloc(sizeof(char *) * (cnt + 1));

    if (!clean)
        return (NULL);
    while (cmd[i])
    {
        if (!ft_strcmp(cmd[i], "<") && cmd[i + 1])
        {
            if (open_infile(cmd[i + 1], in_fd) == -1)
            {
                free(clean);
                return NULL;
            }
            i += 2;
        }
        else if (!ft_strcmp(cmd[i], "<<") && cmd[i + 1])
        {
            if (handle_heredoc(cmd[i + 1], in_fd) == -1)
            {
                free(clean);
                return NULL;
            }
            i += 2;
        }
        else if (!ft_strcmp(cmd[i], ">") && cmd[i + 1])
        {
            if (open_outfile(cmd[i + 1], out_fd) == -1)
            {
                free(clean);
                return NULL;
            }
            i += 2;
        }
        else if (!ft_strcmp(cmd[i], ">>") && cmd[i + 1])
        {
            if (open_appendfile(cmd[i + 1], out_fd) == -1)
            {
                free(clean);
                return NULL;
            }
            i += 2;
        }
        else
            clean[j++] = cmd[i++];
    }
    clean[j] = NULL;
    free(cmd);
    return (clean);
}
