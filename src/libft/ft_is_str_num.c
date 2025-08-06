/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_str_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:29:49 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 20:54:32 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_str_num(const char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (!str || *str == '\0')
		return (0);
	if ((str[0] == '+' || str[0] == '-') && str[1] != '\0')
		i = 1;
	while (str[i] == '0')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		len++;
		i++;
	}
	if (len == 0 && str[i - 1] == '0')
		len = 1;
	if (str[i] != '\0')
		return (0);
	return (len);
}
