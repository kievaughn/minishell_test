/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:54:45 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/13 15:46:51 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	getsize(long n)
{
	unsigned int	size;

	size = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

static void	passtostring(unsigned int size, unsigned int n, char *s,
		short int isnegative)
{
	if (isnegative == 1)
	{
		s[0] = '-';
		while (size > 1)
		{
			s[size - 1] = (n % 10) + '0';
			n = n / 10;
			size--;
		}
	}
	else
	{
		while (size > 0)
		{
			s[size - 1] = (n % 10) + '0';
			n = n / 10;
			size--;
		}
	}
}

char	*ft_itoa(int n)
{
	char			*ret;
	unsigned int	size;
	long			num;
	short int		isnegative;

	num = n;
	isnegative = 0;
	if (num < 0)
	{
		isnegative = 1;
		num = -num;
	}
	size = getsize(num) + isnegative;
	ret = ft_calloc(size + 1, sizeof(char));
	if (!ret)
		return (NULL);
	passtostring(size, num, ret, isnegative);
	return (ret);
}
