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

char    **tokenize_command(char const *s, char c, char **envp)
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
               if(arr[i][0] != '\'')
               {
                       char *expanded = build_expanded_str(arr[i], envp);
                       free(arr[i]);
                       arr[i] = expanded;
               }
               i++;
               s += len;
       }
       arr[i] = NULL;
       arr = split_redirs(arr);
       if (!arr)
               return (NULL);
       i = 0;
       while (arr[i])
       {
               remove_quotes(arr[i]);
               i++;
       }
       return (arr);
}
