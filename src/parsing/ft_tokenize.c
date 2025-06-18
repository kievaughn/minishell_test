#include "minishell.h"
#include "../libft/libft.h"

static char *append_char(char *str, char c)
{
    char tmp[2];
    tmp[0] = c;
    tmp[1] = '\0';
    return ft_strcatrealloc(str, tmp);
}

static char *expand_variable(char *dst, const char **src, char **envp)
{
    const char *s = *src + 1; /* skip $ */
    if (*s == '?')
    {
        char *code = ft_itoa(last_exit_code);
        dst = ft_strcatrealloc(dst, code ? code : "");
        free(code);
        s++;
    }
    else if (ft_isalpha(*s) || *s == '_')
    {
        int len = 0;
        while (ft_isalnum(s[len]) || s[len] == '_')
            len++;
        char *name = ft_substr(s, 0, len);
        char *val = name ? get_env_value(envp, name) : NULL;
        if (val)
            dst = ft_strcatrealloc(dst, val);
        free(name);
        s += len;
    }
    else
    {
        dst = append_char(dst, '$');
    }
    *src = s;
    return dst;
}

char **ft_tokenize(char const *s, char c, char **envp)
{
    size_t max = ft_strlen(s) + 1;
    char **arr = malloc(sizeof(char *) * max);
    size_t i = 0;
    const char *p = s;

    if (!arr)
        return NULL;
    while (*p)
    {
        while (*p == c)
            p++;
        if (!*p)
            break;
        char quote = 0;
        if (*p == '\'' || *p == '"')
        {
            quote = *p;
            p++;
        }
        char *token = NULL;
        while (*p && (quote || *p != c))
        {
            if (!quote && (*p == '\'' || *p == '"'))
            {
                quote = *p;
                p++;
                continue;
            }
            if (quote && *p == quote)
            {
                quote = 0;
                p++;
                continue;
            }
            if (*p == '$' && quote != '\'')
            {
                token = expand_variable(token, &p, envp);
                continue;
            }
            token = append_char(token, *p);
            p++;
        }
        if (quote)
        {
            fprintf(stderr, "Unclosed quote\n");
            while (i > 0)
                free(arr[--i]);
            free(arr);
            exit(1);
        }
        arr[i] = token ? token : ft_strdup("");
        i++;
        if (*p == c)
            p++;
    }
    arr[i] = NULL;
    return arr;
}
