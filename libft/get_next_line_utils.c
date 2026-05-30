/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:01:02 by rgomes-g          #+#    #+#             */
/*   Updated: 2025/11/16 17:02:20 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup_v2(const char *s1)
{
	char	*buffer;
	size_t	len;
	size_t	i;

	if (!s1)
		return (NULL);
	len = ft_strlen_v2(s1);
	buffer = (char *)malloc(len + 1);
	if (!buffer)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		buffer[i] = s1[i];
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}

char	*free_buffer(char **buffer, char **read_buf)
{
	if (buffer && *buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	if (read_buf && *read_buf)
	{
		free(*read_buf);
		*read_buf = NULL;
	}
	return (NULL);
}

void	*ft_calloc_v2(size_t nmemb, size_t size)
{
	size_t			len;
	void			*buffer;
	unsigned char	*ptr;
	size_t			i;

	if (nmemb != 0 && size > ((size_t) - 1) / nmemb)
		return (NULL);
	len = nmemb * size;
	buffer = malloc(len);
	if (!buffer)
		return (NULL);
	ptr = (unsigned char *)buffer;
	i = 0;
	while (i < len)
	{
		ptr[i] = 0;
		i++;
	}
	return (buffer);
}

char	*ft_strjoin_v2(const char *s1, const char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*buffer;
	size_t	i;

	if (!s1 && !s2)
		return (NULL);
	len_s1 = ft_strlen_v2(s1);
	len_s2 = ft_strlen_v2(s2);
	buffer = (char *)malloc(len_s1 + len_s2 + 1);
	if (!buffer)
		return (NULL);
	i = 0;
	while (*s1)
		buffer[i++] = *s1++;
	while (*s2)
		buffer[i++] = *s2++;
	buffer[len_s1 + len_s2] = '\0';
	return (buffer);
}

char	*ft_substr_v2(const char *s, unsigned int start, size_t len)
{
	size_t	slen;
	size_t	i;
	char	*buffer;

	if (!s)
		return (NULL);
	slen = ft_strlen_v2(s);
	if (start >= slen)
		return (ft_strdup_v2(""));
	if ((start + len) > slen)
		len = slen - start;
	buffer = (char *)malloc(len + 1);
	if (!buffer)
		return (NULL);
	i = 0;
	while (i < len)
	{
		buffer[i] = s[start];
		i++;
		start++;
	}
	buffer[i] = '\0';
	return (buffer);
}
