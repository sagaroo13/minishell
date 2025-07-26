/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:08:54 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/03/12 20:29:07 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_line(char *storage)
{
	char	*line;
	int		i;

	if (!storage[0])
		return (NULL);
	i = 0;
	while (storage[i] && storage[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (storage[i] && storage[i] != '\n')
	{
		line[i] = storage[i];
		i++;
	}
	if (storage[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*update_storage(char *storage)
{
	char	*new_storage;
	int		i;
	int		j;

	i = 0;
	while (storage[i] && storage[i] != '\n')
		i++;
	if (!storage[i])
	{
		free(storage);
		return (NULL);
	}
	new_storage = malloc(sizeof(char) * (ft_strlen_gnl(storage) - i + 1));
	if (!new_storage)
		return (NULL);
	i++;
	j = 0;
	while (storage[i])
		new_storage[j++] = storage[i++];
	new_storage[j] = '\0';
	free(storage);
	return (new_storage);
}

char	*read_and_storage(int fd, char *storage)
{
	char	*buffer;
	ssize_t	readen_bytes;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	readen_bytes = 1;
	while (!ft_strchr_gnl(storage, '\n') && readen_bytes != 0)
	{
		readen_bytes = read(fd, buffer, BUFFER_SIZE);
		if (readen_bytes == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[readen_bytes] = '\0';
		storage = ft_strjoin_gnl(storage, buffer);
	}
	free(buffer);
	return (storage);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		if (storage)
			free(storage);
		return (NULL);
	}
	storage = read_and_storage(fd, storage);
	if (!storage)
		return (NULL);
	line = get_line(storage);
	storage = update_storage(storage);
	if (!line)
	{
		free(storage);
		storage = NULL;
	}
	return (line);
<<<<<<< HEAD
}
=======
}
>>>>>>> origin/dediaz-f
