/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:00:54 by rgomes-g          #+#    #+#             */
/*   Updated: 2025/11/16 17:01:58 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*buffers[1024];
	char		*line;

	if (fd == -1 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buffers[fd] = read_to_buffer(fd, buffers[fd]);
	if (!buffers[fd])
		return (NULL);
	line = extract_line(&buffers[fd]);
	if (!line)
		return (free_buffer(&buffers[fd], NULL));
	return (line);
}

char	*read_to_buffer(int fd, char *buffer)
{
	char	*temp;
	char	*read_buf;
	ssize_t	bytes_read;

	bytes_read = 1;
	if (!buffer)
		buffer = ft_calloc_v2(1, sizeof(char));
	read_buf = ft_calloc_v2(BUFFER_SIZE + 1, sizeof(char));
	while (!ft_strchr_v2(buffer, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, read_buf, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free_buffer(&buffer, &read_buf));
		read_buf[bytes_read] = '\0';
		temp = ft_strjoin_v2(buffer, read_buf);
		free_buffer(&buffer, NULL);
		buffer = temp;
	}
	free_buffer(&read_buf, NULL);
	if ((buffer[0] == '\0' || !buffer) && bytes_read == 0)
		return (free_buffer(&buffer, NULL));
	return (buffer);
}

char	*extract_line(char **buffer)
{
	char	*new_buffer;
	char	*line;
	size_t	len;

	if (ft_strchr_v2(*buffer, '\n'))
	{
		len = ft_strchr_v2(*buffer, '\n') - *buffer + 1;
		line = ft_substr_v2(*buffer, 0, len);
		new_buffer = ft_strdup_v2(*buffer + len);
		free_buffer(buffer, NULL);
		*buffer = new_buffer;
	}
	else
	{
		line = ft_strdup_v2(*buffer);
		free_buffer(buffer, NULL);
	}
	return (line);
}

size_t	ft_strlen_v2(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

char	*ft_strchr_v2(const char *s, int c)
{
	size_t	i;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen_v2(s);
	if (c == '\0')
		return ((char *)&s[len]);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (unsigned char)c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}
