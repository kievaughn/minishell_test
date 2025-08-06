/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcatrealloc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:15:10 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/23 19:57:15 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcatrealloc(char *src, const char *new)
{
	char	*newdata;
	int		src_len;
	int		new_len;

	if (!src)
	{
		src = malloc(1);
		if (!src)
			return (NULL);
		src[0] = '\0';
	}
	src_len = ft_strlen(src);
	new_len = ft_strlen(new);
	newdata = malloc(src_len + new_len + 1);
	if (!newdata)
	{
		free(src);
		return (NULL);
	}
	ft_memcpy(newdata, src, src_len);
	ft_memcpy(newdata + src_len, new, new_len + 1);
	free(src);
	return (newdata);
}
