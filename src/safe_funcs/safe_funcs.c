/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:22:19 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 13:25:56 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	safe_chdir(const char *path)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("chdir");
	}
}

void	safe_close(int fd)
{
	if (close(fd) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("close");
	}
}

int	safe_open(const char *path, t_open_flags flags)
{
	int	fd;

	fd = 0;
	if (flags == READ)
		fd = open(path, O_RDONLY);
	else if (flags == WRITE)
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (flags == APPEND)
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
	{
		ft_putendl_fd("minishell: invalid open flags", 2);
		return (-1);
	}
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("open");
	}
	return (fd);
}

void	safe_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("dup2");
	}
}

int	safe_dup(int fd)
{
	int	new_fd;

	new_fd = dup(fd);
	if (new_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("dup");
	}
	return (new_fd);
}
