/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:54:06 by rgomes-g          #+#    #+#             */
/*   Updated: 2025/07/31 18:48:35 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numlen(long int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long int	nb;
	int			digits;
	char		*str;

	nb = n;
	digits = ft_numlen(nb);
	str = ft_calloc(digits + 1, sizeof(char));
	if (!str)
		return (NULL);
	if (nb < 0)
		nb = -nb;
	while (digits > 0)
	{
		str[--digits] = nb % 10 + '0';
		nb /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
