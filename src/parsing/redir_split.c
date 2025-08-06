/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:18:50 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/01 00:00:00 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static void update_quote_state(char c, char *quote)
{
    if (!*quote && (c == '\'' || c == '"'))
        *quote = c;
    else if (*quote && c == *quote)
        *quote = 0;
}

static void append_operator(char *str, char **out, int *idx, int *j)
{
    if (str[*j] == '>' && str[*j + 1] == '>')
    {
        out[*idx] = ft_strdup(">>");
        (*idx)++;
        *j += 2;
    }
    else if (str[*j] == '<' && str[*j + 1] == '<')
    {
        out[*idx] = ft_strdup("<<");
        (*idx)++;
        *j += 2;
    }
    else
    {
        out[*idx] = ft_substr(str, *j, 1);
        (*idx)++;
        (*j)++;
    }
}

static int  append_redir_parts(char *str, char **out, int idx)
{
    int     j;
    int     start;
    char    quote;

    j = 0;
    start = 0;
    quote = 0;
    while (str[j])
    {
        update_quote_state(str[j], &quote);
        if (!quote && (str[j] == '>' || str[j] == '<'))
        {
            if (j - start > 0)
                out[idx++] = ft_substr(str, start, j - start);
            append_operator(str, out, &idx, &j);
            start = j;
        }
        else
            j++;
    }
    if (j - start > 0)
        out[idx++] = ft_substr(str, start, j - start);
    return (idx);
}

char    **split_redirs(char **arr)
{
    char    **out;
    int     i;
    int     idx;

    out = malloc(sizeof(char *) * (total_parts(arr) + 1));
    if (!out)
    {
        free_cmd(arr);
        return (NULL);
    }
    idx = 0;
    i = 0;
    while (arr[i])
    {
        idx = append_redir_parts(arr[i], out, idx);
        free(arr[i]);
        i++;
    }
    out[idx] = NULL;
    free(arr);
    return (out);
}

