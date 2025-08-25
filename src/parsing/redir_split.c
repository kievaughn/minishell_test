#include "../libft/libft.h"
#include "minishell.h"

static int	part_count(char *tok)
{
	int	i;
	int	start;
	int	count;

	i = 0;
	start = 0;
	count = 0;
	while (tok[i])
	{
		if (tok[i] == '>' || tok[i] == '<')
		{
			if (i - start > 0)
				count++;
			if (tok[i] == '>' && tok[i + 1] == '>')
			{
				count++;
				i += 2;
			}
			else
			{
				count++;
				i++;
			}
			start = i;
		}
		else
			i++;
	}
	if (i - start > 0)
		count++;
	return (count);
}

static int total_parts(t_token **arr)
{
    int total;
    int i;

    total = 0;
    i = 0;
    while (arr && arr[i])
    {
        total += part_count(arr[i]->str);
        i++;
    }
    return (total);
}


t_token **split_redirs(t_token **arr)
{
    t_token **out;
    int i, j, idx, start;
    char quote;

    out = malloc(sizeof(t_token *) * (total_parts(arr) + 1));
    if (!out)
        return (free_tokens(arr), NULL);

    idx = 0;
    i = 0;
    while (arr[i])
    {
        j = 0;
        start = 0;
        quote = 0;
        while (arr[i]->str[j])
        {
            if (!quote && (arr[i]->str[j] == '\'' || arr[i]->str[j] == '"'))
                quote = arr[i]->str[j];
            else if (quote && arr[i]->str[j] == quote)
                quote = 0;

            if (!quote && (arr[i]->str[j] == '>' || arr[i]->str[j] == '<'))
            {
                if (j - start > 0)
                    out[idx++] = new_token(ft_substr(arr[i]->str, start, j - start),
                                           arr[i]->quoted, 0);
                if (arr[i]->str[j] == '>' && arr[i]->str[j+1] == '>')
                {
                    out[idx++] = new_token(ft_strdup(">>"), 0, 1);
                    j += 2;
                }
                else if (arr[i]->str[j] == '<' && arr[i]->str[j+1] == '<')
                {
                    out[idx++] = new_token(ft_strdup("<<"), 0, 2);
                    j += 2;
                }
                else if (arr[i]->str[j] == '>')
                {
                    out[idx++] = new_token(ft_strdup(">"), 0, 3);
                    j++;
                }
                else
                {
                    out[idx++] = new_token(ft_strdup("<"), 0, 4);
                    j++;
                }
                start = j;
            }
            else
                j++;
        }
        if (j - start > 0)
            out[idx++] = new_token(ft_substr(arr[i]->str, start, j - start),
                                   arr[i]->quoted, 0);
        free(arr[i]->str);
        free(arr[i]);
        i++;
    }
    out[idx] = NULL;
    free(arr);
    return (out);
}

