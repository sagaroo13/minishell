/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:04:39 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 09:09:15 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	attach_last_heredoc_to_stdin(int last_fd)
{
	if (last_fd != -1)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
}

static bool	process_heredoc_line(char *line, char *delim,
		int pipe_fd[2])
{
	char	*nl;

	if (!line)
		return (false);
	nl = ft_strchr(line, '\n');
	if (nl)
		*nl = '\0';
	if (!ft_strcmp(delim, line))
	{
		free(line);
		return (false);
	}
	ft_putendl_fd(line, pipe_fd[1]);
	free(line);
	return (true);
}

void	read_from_stdin(int pipe_fd[2], char *delim)
{
	int		tty_fd;
	int		in_fd;
	char	*line;

	close(pipe_fd[0]);
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd >= 0)
		in_fd = tty_fd;
	else
		in_fd = STDIN_FILENO;
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(in_fd);
		if (!line || !process_heredoc_line(line, delim, pipe_fd))
			break;
	}
	if (tty_fd >= 0)
		close(tty_fd);
	close(pipe_fd[1]);
	get_next_line(-1);
	exit(EXIT_SUCCESS);
}
