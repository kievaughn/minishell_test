/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:12:49 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 13:24:30 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../libft/libft.h"
#include "minishell.h"

static int  fully_quoted(const char *s)
{
    size_t  len;
    size_t  i;
    char    quote;

    if (!s)
        return (0);
    len = ft_strlen(s);
    if (len < 2)
        return (0);
    quote = s[0];
    if ((quote != '\'' && quote != '"') || s[len - 1] != quote)
        return (0);
    i = 1;
    while (i < len - 1)
    {
        if (s[i] == quote)
            return (0);
        i++;
    }
    if (quote == '\'')
        return (1);
    return (2);
}

static int  is_delim(char ch, char delim)
{
    if (delim == ' ')
        return (ch == ' ' || ch == '\t');
    return (ch == delim);
}

static size_t skip_token(const char *s, size_t i, char c)
{
    char quote = 0;

    while (s[i])
    {
        if (!quote && (s[i] == '"' || s[i] == '\''))
        {
            quote = s[i++];
            while (s[i] && s[i] != quote)
                i++;
            if (s[i] == quote)
                i++;
            quote = 0;
            continue;
        }
        if (!quote && is_delim(s[i], c))
            break;
        i++;
    }
    return i;
}

static size_t token_count(const char *s, char c)
{
    size_t i = 0;
    size_t count = 0;

    while (s[i])
    {
        while (is_delim(s[i], c))
            i++;
        if (!s[i])
            break;
        count++;
        i = skip_token(s, i, c);
    }
    return count;
}

static size_t next_c(const char *s, char c)
{
    size_t i = 0;
    char quote = 0;

    while (s[i])
    {
        if (!quote && (s[i] == '"' || s[i] == '\''))
        {
            quote = s[i++];
            while (s[i] && s[i] != quote)
                i++;
            if (s[i] == quote)
                i++;
            quote = 0;
            continue;
        }
        if (!quote && is_delim(s[i], c))
            break;
        i++;
    }
    return i;
}

static t_token **fill_arr_from_string(const char *s, char c)
{
    t_token **arr;
    int token_num;
    int i;
    size_t len;
    int type;
    int quoted;
    char *substr;

    token_num = token_count(s, c);
    arr = malloc(sizeof(t_token *) * (token_num + 1));
    if (!arr)
        return (NULL);

    i = 0;
    while (*s)
    {
        while (is_delim(*s, c))
            s++;
        if (!*s)
            break;

        len = next_c(s, c);
        substr = ft_substr(s, 0, len);
        if (!substr)
        {
            arr[i] = NULL;          // ensure free_tokens sees a terminator
            free_tokens(arr);
            return (NULL);
        }

        // use the robust detector
        quoted = fully_quoted(substr);

        type = 0;
        arr[i++] = new_token(substr, quoted, type);
        free(substr);

        s += len;
    }
    arr[i] = NULL;
    return arr;
}

t_token **tokenize_command(char const *s, char c, char **envp)
{
    t_token **arr;
    int i;

    if (!s)
        return NULL;

    arr = fill_arr_from_string(s, c);
    if (!arr)
        return NULL;

    for (i = 0; arr[i]; i++)
    {
        if (arr[i]->quoted != 1)  // not single quoted
        {
            char *expanded = build_expanded_str(arr[i]->str, envp);
            if (!expanded)
            {
                free_tokens(arr);
                return NULL;
            }
            free(arr[i]->str);
            arr[i]->str = expanded;
        }
        // remove surrounding quotes after expansion (or for single quotes)
        remove_quotes(arr[i]->str);
    }

    // 3. Split expansions if needed
    arr = split_expanded_tokens(arr);
    if (!arr)
        return NULL;

    // 4. Handle redirections
    arr = split_redirs(arr);
    if (!arr)
        return NULL;

    return arr;
}

t_token *new_token(const char *str, int quoted, int type)
{
    t_token *tok = malloc(sizeof(t_token));
    if (!tok)
        return NULL;
    tok->str = ft_strdup(str);
    if (!tok->str)
    {
        free(tok);
        return NULL;
    }
    tok->quoted = quoted;
    tok->type = type;
    return tok;
}




