#include "minishell.h"
#include "../libft/libft.h"
#include <fcntl.h>

char **handle_redirections(char **cmd, int *in_fd, int *out_fd)
{
    int i = 0;
    int j = 0;
    int count = count_strings(cmd);
    char **clean = malloc(sizeof(char *) * (count + 1));
    if (!clean)
        return NULL;
    while (cmd[i])
    {
        if (!ft_strncmp(cmd[i], "<", 2) && cmd[i + 1])
        {
            if (*in_fd != STDIN_FILENO)
                close(*in_fd);
            *in_fd = open(cmd[i + 1], O_RDONLY);
            if (*in_fd < 0)
                perror(cmd[i + 1]);
            free(cmd[i]);
            free(cmd[i + 1]);
            i += 2;
            continue;
        }
        if (!ft_strncmp(cmd[i], ">", 2) && cmd[i + 1])
        {
            if (*out_fd != STDOUT_FILENO)
                close(*out_fd);
            *out_fd = open(cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*out_fd < 0)
                perror(cmd[i + 1]);
            free(cmd[i]);
            free(cmd[i + 1]);
            i += 2;
            continue;
        }
        if (!ft_strncmp(cmd[i], ">>", 3) && cmd[i + 1])
        {
            if (*out_fd != STDOUT_FILENO)
                close(*out_fd);
            *out_fd = open(cmd[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*out_fd < 0)
                perror(cmd[i + 1]);
            free(cmd[i]);
            free(cmd[i + 1]);
            i += 2;
            continue;
        }
        clean[j++] = cmd[i];
        i++;
    }
    clean[j] = NULL;
    free(cmd);
    return clean;
}
