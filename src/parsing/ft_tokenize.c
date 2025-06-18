/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:16:21 by kbrandon          #+#    #+#             */
/*   Updated: 2025/06/18 19:56:35 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

static size_t	token_count(char const *s, char c)
{
        size_t  i;
        size_t  count;
        char    quote;
		
        i = 0;
        count = 0;
        while (s[i])
        {
                while (s[i] == c)
                        i++;
                if (!s[i])
                        break ;
                count++;
                quote = 0;
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
                                continue ;
                        }
                        if (!quote && s[i] == c)
                                break ;
                        i++;
                }
        }
        return (count);
}

static void	free_arr(char **arr, int i)
{
	while (i-- > 0)
		free(arr[i]);
	free(arr);
}
static size_t   next_c(char *s, char c)
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
static void     remove_quotes(char *str)
{
        size_t  i;
        size_t  j;
        char    quote;

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

char    **ft_tokenize(char const *s, char c)
{
        char    **arr;
        int             i;
        size_t  len;

        if (!s)
                return (NULL);
        arr = (char **)malloc((token_count(s, c) + 1) * sizeof(char *));
        if (!arr)
                return (NULL);
        i = 0;
        while (*s)
        {
                while (*s == c)
                        s++;
                if (!*s)
                        break ;
                len = next_c((char *)s, c);
                arr[i] = ft_substr((char *)s, 0, len);
                if (!arr[i])
                        return (free_arr(arr, i), NULL);
                remove_quotes(arr[i]);
                i++;
                s += len;
        }
        arr[i] = NULL;
        return (arr);
}
