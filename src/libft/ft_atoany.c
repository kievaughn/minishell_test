/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:55:01 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 20:26:55 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <ctype.h>
#include <limits.h>

int checkoverflow(const char *str, int *i, int sign, long long *result)
{
    long long res;
    int digit;

    res = *result;
    while (str[*i] >= '0' && str[*i] <= '9')
	{
        digit = str[*i] - '0';
        if (sign == 1) {
            if (res > (LLONG_MAX - digit) / 10)
                return (0);
        } else {
            if ((unsigned long long)res > ((unsigned long long)(-(LLONG_MIN + digit))) / 10)
                return (0);
        }
        res = res * 10 + digit;
        (*i)++;
    }
    *result = res;
    return (1);
}

int ft_atoany(const char *str, long long *out)
{
    long long result;
    int sign;
    int i;

    result = 0;
    sign = 1;
    i = 0;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    if (str[i] < '0' || str[i] > '9')
        return 0;
    if (!checkoverflow(str, &i, sign, &result))
        return 0;
    *out = result * sign;
    return (1);
}


