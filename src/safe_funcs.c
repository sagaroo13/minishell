/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:23:42 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/07/08 17:23:56 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*safe_malloc(size_t size, bool calloc_flag)
{
	void	*ptr;

	if (size == 0)
		return (NULL);
	if (calloc_flag)
		ptr = ft_calloc(1, size);
	else
		ptr = malloc(size);
	if (!ptr)
		perror(RED "[ERROR]" RESET " Memory allocation failed.");
	return (ptr);
}

void	safe_getcwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror(RED "[ERROR]" RESET " getcwd failed.");
}

void	safe_chdir(const char *path)
{
	if (chdir(path) == -1)
		perror(RED "[ERROR]" RESET " chdir failed.");
}

void	safe_close(int fd)
{
	if (close(fd) == -1)
		perror(RED "[ERROR]" RESET " close failed.");
}

int	safe_open(const char *path, t_open_flags flags)
{
	int	fd;

	if (flags == READ)
		fd = open(path, O_RDONLY);
	else if (flags == WRITE)
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (flags == APPEND)
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
	{
		perror(RED "[ERROR]" RESET " Invalid open flags.");
		return (-1);
	}
	if (fd == -1)
		perror(RED "[ERROR]" RESET " open failed.");
	return (fd);
}

void	safe_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
		perror(RED "[ERROR]" RESET " dup2 failed.");
}

int	safe_dup(int fd)
{
	int	new_fd;

	new_fd = dup(fd);
	if (new_fd == -1)
		perror(RED "[ERROR]" RESET " dup failed.");
	return (new_fd);
}
