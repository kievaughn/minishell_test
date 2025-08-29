#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../includes/minishell.h"
#include "../src/libft/libft.h"

int g_exit_code = 0;

static void run_case(const char *input, const char *expected, char **env)
{
    char *dup = ft_strdup(input);
    char *res = build_expanded_str(dup, env);
    free(dup);
    if (res)
        remove_quotes(res);
    printf("input: %s -> %s\n", input, res ? res : "(null)");
    assert(res && strcmp(res, expected) == 0);
    free(res);
}

int main(void)
{
    char *env[] = {"USER=testuser", NULL};
    run_case("$USER", "testuser", env);
    run_case("\"$USER\"", "testuser", env);
    run_case("$US\"E\"R", "ER", env);
    run_case("$U'S'E'R", "SER", env);
    run_case("$\"HOME\"$USER", "HOMEtestuser", env);
    run_case("$\"HOM\"E$USER", "HOMEtestuser", env);
    run_case("$\"HOME\"", "HOME", env);
    run_case("$\"42$\"", "42$", env);
    printf("All expansion tests passed\n");
    return 0;
}
