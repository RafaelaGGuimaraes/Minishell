/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 19:26:30 by rgomes-g          #+#    #+#             */
/*   Updated: 2025/07/31 18:47:02 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*ptrdest;
	const unsigned char	*ptrsrc;

	ptrdest = (unsigned char *)dest;
	ptrsrc = (const unsigned char *)src;
	i = 0;
	if (dest == src)
		return (dest);
	if ((ptrsrc < ptrdest) && (ptrsrc + n > ptrdest))
	{
		while (n > 0)
		{
			n--;
			ptrdest[n] = ptrsrc[n];
		}
	}
	while (i < n)
	{
		ptrdest[i] = ptrsrc[i];
		i++;
	}
	return (dest);
}
