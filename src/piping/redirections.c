#include "minishell.h"
#include "../libft/libft.h"
#include <fcntl.h>
#include <readline/readline.h>

static void open_infile(char *file, int *in_fd)
{
    int fd;

    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    fd = open(file, O_RDONLY);
    if (fd < 0)
        perror(file);
    else
        *in_fd = fd;
}

static void open_outfile(char *file, int *out_fd)
{
    int fd;

    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        perror(file);
    else
        *out_fd = fd;
}

static void open_appendfile(char *file, int *out_fd)
{
    int fd;

    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
        perror(file);
    else
        *out_fd = fd;
}

static int open_heredoc(const char *delim)
{
    int  pipefd[2];
    char *line;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
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
    return (pipefd[0]);
}

static void handle_heredoc(const char *delim, int *in_fd)
{
    int h = open_heredoc(delim);

    if (h >= 0)
    {
        if (*in_fd != STDIN_FILENO)
            close(*in_fd);
        *in_fd = h;
    }
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
            open_infile(cmd[i + 1], in_fd), i += 2;
        else if (!ft_strcmp(cmd[i], "<<") && cmd[i + 1])
            handle_heredoc(cmd[i + 1], in_fd), i += 2;
        else if (!ft_strcmp(cmd[i], ">") && cmd[i + 1])
            open_outfile(cmd[i + 1], out_fd), i += 2;
        else if (!ft_strcmp(cmd[i], ">>") && cmd[i + 1])
            open_appendfile(cmd[i + 1], out_fd), i += 2;
        else
            clean[j++] = cmd[i++];
    }
    clean[j] = NULL;
    free(cmd);
    return (clean);
}
