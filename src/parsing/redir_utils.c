#include "../libft/libft.h"
#include "minishell.h"

static int	handle_op(char *tok, int *i)
{
	if (tok[*i] == '>' && tok[*i + 1] == '>')
	{
		*i += 2;
		return (1);
	}
	(*i)++;
	return (1);
}

static int	part_count(char *tok)
{
	int	i;
	int	start;
	int	count;

	if (!tok[0])
		return (1);

	i = 0;
	start = 0;
	count = 0;
	while (tok[i])
	{
		if (tok[i] == '>' || tok[i] == '<')
		{
			if (i - start > 0)
				count++;
			count += handle_op(tok, &i);
			start = i;
		}
		else
			i++;
	}
	if (i - start > 0)
		count++;
	return (count);
}

int	total_parts(char **arr)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (arr && arr[i])
	{
		total += part_count(arr[i]);
		i++;
	}
	return (total);
}
