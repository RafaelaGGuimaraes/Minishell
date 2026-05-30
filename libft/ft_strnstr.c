/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 19:28:08 by rgomes-g          #+#    #+#             */
/*   Updated: 2025/07/31 18:47:52 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(char const *haystack, char const *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (needle[j] == '\0')
		return ((char *) haystack);
	while (haystack[i] && i < len)
	{
		j = 0;
		if (haystack[i] == needle[j])
		{
			while (haystack[(i + j)] != '\0' && needle[j] != '\0'
				&& (haystack[(i + j)] == needle[j]) && (i + j) < len)
			{
				j++;
			}
			if (needle[j] == '\0')
				return ((char *) haystack + i);
		}
		i++;
	}
	return (NULL);
}
