#include "minishell.h"
#include "libft/libft.h"

short int custom_cd(char **envp, char **args)
{
    char *target_dir;

	target_dir = NULL;
    if (!args[1] || ft_strlen(args[1]) == 0)
    {
        target_dir = get_env_value(envp, "HOME");
        if (!target_dir)
        {
            printf("cd: HOME not set\n");
            return (1);
        }
    }
    else
    {
        target_dir = args[1];
    }
    if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (1);
	}
    return (0);
}

short int custom_exit(char **args)
{
    long long value;

    printf("exit\n");

    if (!args || !args[1])
        exit(0);

    if (!ft_atoany(args[1], &value))
    {
        printf("exit: %s: numeric argument required\n", args[1]);
        exit(255);
    }
    if (args[2])
    {
        printf("exit: too many arguments\n");
        return (1);
    }
    exit((unsigned char)(value % 256));
}

