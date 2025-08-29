/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:12:49 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/27 12:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../libft/libft.h"
#include "minishell.h"
#include "token_utils.h"

static t_token  **alloc_token_array(const char *s, char c, int *token_num)
{
        t_token **arr;

        *token_num = token_count(s, c);
        arr = malloc(sizeof(t_token *) * (*token_num + 1));
        if (!arr)
                return (NULL);
        return (arr);
}

static int      populate_tokens(t_token **arr, const char *s, char c)
{
        int             i;
        size_t  len;
        int             type;
        int             quoted;
        char    *substr;

        i = 0;
        while (*s)
        {
                while (is_delim(*s, c))
                        s++;
                if (!*s)
                        break ;
                len = next_c(s, c);
                substr = ft_substr(s, 0, len);
                if (!substr)
                {
                        arr[i] = NULL;
                        return (1);
                }
                quoted = fully_quoted(substr);
                type = 0;
                arr[i] = new_token(substr, quoted, type);
                free(substr);
                if (!arr[i])
                        return (1);
                s += len;
                i++;
        }
        arr[i] = NULL;
        return (0);
}

static t_token  **fill_arr_from_string(const char *s, char c)
{
        t_token **arr;
        int             token_num;

        arr = alloc_token_array(s, c, &token_num);
        if (!arr)
                return (NULL);
        (void)token_num;
        if (populate_tokens(arr, s, c))
        {
                free_tokens(arr);
                return (NULL);
        }
        return (arr);
}

static int      expand_all(t_token **arr, char **envp)
{
        int             i;
        char    *expanded;

        i = 0;
        while (arr && arr[i])
        {
                if (arr[i]->quoted != 1)
                {
                        expanded = build_expanded_str(arr[i]->str, envp);
                        if (!expanded)
                                return (1);
                        free(arr[i]->str);
                        arr[i]->str = expanded;
                }
                if (ft_strchr(arr[i]->str, '"') || ft_strchr(arr[i]->str, '\''))
                        remove_quotes(arr[i]->str);
                i++;
        }
        return (0);
}

static t_token  **post_process_tokens(t_token **arr)
{
        arr = split_expanded_tokens(arr);
        if (!arr)
                return (NULL);
        arr = split_redirs(arr);
        if (!arr)
                return (NULL);
        return (arr);
}

t_token **tokenize_command(char const *s, char c, char **envp)
{
        t_token **arr;

        if (!s)
                return (NULL);
        if (has_unclosed_quotes(s))
        {
                error_msg("minishell", "unexpected EOF while looking for matching quote");
                g_exit_code = 2;
                return (NULL);
        }
        arr = fill_arr_from_string(s, c);
        if (!arr)
                return (NULL);
        if (expand_all(arr, envp))
        {
                free_tokens(arr);
                return (NULL);
        }
        return (post_process_tokens(arr));
}

t_token *new_token(const char *str, int quoted, int type)
{
        t_token *tok;

        tok = malloc(sizeof(t_token));
        if (!tok)
                return (NULL);
        tok->str = ft_strdup(str);
        if (!tok->str)
        {
                free(tok);
                return (NULL);
        }
        tok->quoted = quoted;
        tok->type = type;
        return (tok);
}

