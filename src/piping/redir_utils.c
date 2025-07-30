#include "minishell.h"
#include <errno.h>

int     open_infile(char *file, int *in_fd)
{
        int     fd;

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
                return (-1);
        }
        *in_fd = fd;
        return (0);
}

int     open_outfile(char *file, int *out_fd)
{
        int     fd;

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
                return (-1);
        }
        *out_fd = fd;
        return (0);
}

int     open_appendfile(char *file, int *out_fd)
{
        int     fd;

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
                return (-1);
        }
        *out_fd = fd;
        return (0);
}
