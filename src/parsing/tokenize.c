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
        if (!quote && s[i] == c)
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
        while (s[i] == c)
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
        if (!quote && s[i] == c)
            break;
        i++;
    }
    return i;
}

static t_token **fill_arr_from_string(const char *s, char c, char **envp)
{
    t_token **arr;
    int token_num;
    int i;
    size_t len;
    char *expanded;
    int type;
    int quoted;

    token_num = token_count(s, c);
    arr = malloc(sizeof(t_token *) * (token_num + 1));
    if (!arr)
        return (NULL);

    i = 0;
    while (*s)
    {
        while (*s == c)
            s++;
        if (!*s)
            break;

        len = next_c(s, c);
        char *substr = ft_substr(s, 0, len);
        if (!substr)
            return (free_tokens(arr), NULL);

        // detect quotes
        quoted = 0;
        if (substr[0] == '\'' && substr[ft_strlen(substr) - 1] == '\'')
            quoted = 1;
        else if (substr[0] == '"' && substr[ft_strlen(substr) - 1] == '"')
            quoted = 2;

        // strip quotes before expansion
        remove_quotes(substr);

        // expand unless single-quoted
        if (quoted != 1)
        {
            expanded = build_expanded_str(substr, envp);
            free(substr);
            if (!expanded)
                return (free_tokens(arr), NULL);
            substr = expanded;
        }

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

    arr = fill_arr_from_string(s, c, envp);
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




