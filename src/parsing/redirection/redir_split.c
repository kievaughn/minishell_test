#include "../../libft/libft.h"
#include "minishell.h"

static int advance_redirect(char *str, int *i, int *count)
{
    if (str[*i] == '>' && str[*i + 1] == '>')
    {
        (*count)++;
        *i += 2;
        return (1);
    }
    if (str[*i] == '<' && str[*i + 1] == '<')
    {
        (*count)++;
        *i += 2;
        return (1);
    }
    if (str[*i] == '>' || str[*i] == '<')
    {
        (*count)++;
        (*i)++;
        return (1);
    }
    return (0);
}

static int part_count(t_token *tok)
{
    int i;
    int start;
    int count;
    int prev;

    if (tok->quoted)
        return (1);
    i = 0;
    start = 0;
    count = 0;
    while (tok->str[i])
    {
        prev = i;
        if (advance_redirect(tok->str, &i, &count))
        {
            if (prev - start > 0)
                count++;
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
        total += part_count(arr[i]);
        i++;
    }
    return (total);
}

static void copy_literal(t_token *tok, t_token **out, int *idx, int start, int end)
{
    char    *part;

    if (end - start <= 0)
        return ;
    part = ft_substr(tok->str, start, end - start);
    if (!part)
        return ;
    out[(*idx)++] = new_token(part, tok->quoted, 0);
    free(part);
}

static void emit_operator(const char *str, int *j, t_token **out, int *idx)
{
    if (str[*j] == '>' && str[*j + 1] == '>')
    {
        out[(*idx)++] = new_token(">>", 0, 4);
        *j += 2;
    }
    else if (str[*j] == '<' && str[*j + 1] == '<')
    {
        out[(*idx)++] = new_token("<<", 0, 2);
        *j += 2;
    }
    else if (str[*j] == '>')
    {
        out[(*idx)++] = new_token(">", 0, 3);
        (*j)++;
    }
    else
    {
        out[(*idx)++] = new_token("<", 0, 1);
        (*j)++;
    }
}

static void split_token(t_token *tok, t_token **out, int *idx)
{
    int j = 0;
    int start = 0;
    char quote = 0;
    if (tok->quoted) { out[(*idx)++] = tok; return ; }
    while (tok->str[j])
    {
        if (!quote && (tok->str[j] == '\'' || tok->str[j] == '"'))
            quote = tok->str[j];
        else if (quote && tok->str[j] == quote)
            quote = 0;
        else if (!quote && (tok->str[j] == '>' || tok->str[j] == '<'))
        {
            copy_literal(tok, out, idx, start, j);
            emit_operator(tok->str, &j, out, idx);
            start = j;
            continue ;
        }
        j++;
    }
    copy_literal(tok, out, idx, start, j);
    free(tok->str);
    free(tok);
}

t_token **split_redirs(t_token **arr)
{
    t_token **out;
    int     i;
    int     idx;

    out = malloc(sizeof(t_token *) * (total_parts(arr) + 1));
    if (!out)
        return (free_tokens(arr), NULL);
    i = 0;
    idx = 0;
    while (arr && arr[i])
        split_token(arr[i++], out, &idx);
    out[idx] = NULL;
    free(arr);
    return (out);
}
