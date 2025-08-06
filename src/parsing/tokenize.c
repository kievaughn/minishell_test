/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:18:59 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static void free_arr(char **arr, int i)
{
    while (i-- > 0)
        free(arr[i]);
    free(arr);
}

static void free_words(char **words)
{
    int j;

    if (!words)
        return;
    j = 0;
    while (words[j])
    {
        free(words[j]);
        j++;
    }
    free(words);
}

static size_t next_c(char *s, char c)
{
    size_t  len;
    char    quote;

    len = 0;
    quote = 0;
    while (s[len])
    {
        if (!quote && (s[len] == '"' || s[len] == '\''))
        {
            quote = s[len++];
            while (s[len] && s[len] != quote)
                len++;
            if (s[len] == quote)
                len++;
            quote = 0;
            continue ;
        }
        if (!quote && s[len] == c)
            break ;
        len++;
    }
    return (len);
}

char    **tokenize_command(char const *s, char c, char **envp)
{
    char    **arr;
    size_t  idx;
    size_t  len;
    char    *tok;
    char    *expanded;
    char    **split;
    int     j;
    int     had_quotes;

    if (!s)
        return (NULL);
    arr = (char **)malloc((ft_strlen(s) + 1) * sizeof(char *));
    if (!arr)
        return (NULL);
    idx = 0;
    while (*s)
    {
        while (*s == c)
            s++;
        if (!*s)
            break ;
        len = next_c((char *)s, c);
        tok = ft_substr((char *)s, 0, len);
        if (!tok)
            return (free_arr(arr, idx), NULL);
        had_quotes = (ft_strchr(tok, '\'') || ft_strchr(tok, '"'));
        if (!ft_strchr(tok, '\''))
        {
            expanded = build_expanded_str(tok, envp);
            free(tok);
            tok = expanded;
            if (!tok)
                return (free_arr(arr, idx), NULL);
        }
        if (!had_quotes)
        {
            split = ft_split(tok, ' ');
            free(tok);
            if (!split)
                return (free_arr(arr, idx), NULL);
            j = 0;
            while (split[j])
            {
                arr[idx] = ft_strdup(split[j]);
                if (!arr[idx])
                {
                    free_arr(arr, idx);
                    free_words(split);
                    return (NULL);
                }
                idx++;
                j++;
            }
            free_words(split);
        }
        else
            arr[idx++] = tok;
        s += len;
    }
    arr[idx] = NULL;
    arr = split_redirs(arr);
    if (!arr)
        return (NULL);
    idx = 0;
    while (arr[idx])
    {
        remove_quotes(arr[idx]);
        idx++;
    }
    return (arr);
}

