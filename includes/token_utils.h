#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <stddef.h>

int     fully_quoted(const char *s);
int     is_delim(char ch, char delim);
size_t  skip_token(const char *s, size_t i, char c);
size_t  token_count(const char *s, char c);
size_t  next_c(const char *s, char c);
int     has_unclosed_quotes(const char *s);

#endif
