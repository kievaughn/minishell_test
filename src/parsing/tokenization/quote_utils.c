#include "token_utils.h"

int     has_unclosed_quotes(const char *s)
{
        char    quote;

        quote = 0;
        while (s && *s)
        {
                if (!quote && (*s == '\'' || *s == '"'))
                        quote = *s;
                else if (quote && *s == quote)
                        quote = 0;
                s++;
        }
        return (quote != 0);
}

