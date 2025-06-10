#include "minishell.h"
#include "libft/libft.h"

void process_command(char **envp, char *line)
{
    char **cmd = ft_split(line, ' ');
    char *path;

    if (!cmd || !cmd[0])
    {
        free_cmd(cmd);
        return;
    }

    if (is_builtin(cmd[0]))
        run_builtin(envp, cmd);
    else
    {
        path = get_path(envp, cmd);
        if (path)
        {
            execute_command(path, cmd, envp);
            free(path);
        }
        else
            printf("Command not found: %s\n", cmd[0]);
    }

    free_cmd(cmd);
}
