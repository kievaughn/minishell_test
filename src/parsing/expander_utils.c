#include "../libft/libft.h"
#include "minishell.h"

void	remove_quotes(char *str)
{
	size_t	i;
	size_t	j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '"' || str[i] == '\''))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				str[j++] = str[i++];
			if (str[i] == quote)
				i++;
			quote = 0;
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
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
