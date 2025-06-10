/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:16:21 by kbrandon          #+#    #+#             */
/*   Updated: 2025/06/10 16:02:37 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	token_count(char const *s, char c)
{
	size_t	i;
	size_t	tkn_ct;
	size_t	ct_flag;

	i = 0;
	tkn_ct = 0;
	ct_flag = 0;
	while (s[i])
	{
		if (s[i] != c && ct_flag == 0)
		{
			tkn_ct++;
			ct_flag = 1;
		}
		else if (s[i] == c)
			ct_flag = 0;
		i++;
	}
	return (tkn_ct);
}

static size_t	next_c(char *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static void	free_arr(char **arr, int i)
{
	while (i-- > 0)
		free(arr[i]);
	free(arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		i;
	size_t	len;

	i = 0;
	len = 0;
	arr = (char **)malloc((token_count(s, c) + 1) * sizeof(char *));
	if (!s || !arr)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			len = next_c((char *)s, c);
			arr[i] = ft_substr((char *)s, 0, len);
			if (!arr[i])
				return (free_arr(arr, i), NULL);
			i++;
			s += len;
		}
	}
	arr[i] = NULL;
	return (arr);
}
