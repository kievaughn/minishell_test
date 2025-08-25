#include "../libft/libft.h"
#include "minishell.h"

// Only removes surrounding quotes if both ends match
void remove_quotes(char *str)
{
    size_t len = ft_strlen(str);

    if (len >= 2 &&
        ((str[0] == '"' && str[len - 1] == '"') ||
         (str[0] == '\'' && str[len - 1] == '\'')))
    {
        memmove(str, str + 1, len - 2);  // shift left
        str[len - 2] = '\0';
    }
}


char	*append_literal(char *result, char *str, int start, int i)
{
	char	*tmp;

	str[i] = '\0';
	tmp = ft_strcatrealloc(result, str + start);
	str[i] = '$';
	if (!tmp)
	{
		free(result);
		return (NULL);
	}
	return (tmp);
}

char	*expand_var(char *str, int *var_len)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && ft_isalnum(str[i]))
		i++;
	*var_len = i;
	if (i > 0)
		return (ft_substr(str, 0, i));
	return (NULL);
}

char	*append_expanded_var(char *result, char *str, int *i, char **envp)
{
	int		var_len;
	char	*var;
	char	*value;
	char	*tmp;

	var_len = 0;
	var = expand_var(&str[*i + 1], &var_len);
	value = get_env_value(envp, var);
	if (!value)
		value = "";
	tmp = ft_strcatrealloc(result, value);
	free(var);
	if (!tmp)
		return (NULL);
	*i += var_len + 1;
	return (tmp);
}
