#include "minishell.h"
#include "libft/libft.h"

void run_builtin(char **envp, char **cmd)
{
    if (!ft_strncmp(cmd[0], "cd", 3))
        custom_cd(envp, cmd);
    else if (!ft_strncmp(cmd[0], "exit", 5))
        custom_exit(cmd);
/*     else if (!ft_strncmp(cmd[0], "echo", 5))
        custom_echo(cmd);
    else if (!ft_strncmp(cmd[0], "pwd", 4))
        custom_pwd();
    else if (!ft_strncmp(cmd[0], "export", 7))
        custom_export(envp, cmd);
    else if (!ft_strncmp(cmd[0], "unset", 6))
        custom_unset(envp, cmd);
    else if (!ft_strncmp(cmd[0], "env", 4))
        custom_env(envp); */
}

