/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:04:39 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 11:01:36 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	attach_last_heredoc_to_stdin(int last_fd)
{
	if (last_fd != -1)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
}

static bool	heredoc_loop(int pipe_fd[2], char *delim,
	bool *eof_reached)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (g_signal_received)
			break ;
		if (!line)
		{
			*eof_reached = true;
			break ;
		}
		if (!ft_strcmp(delim, line))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}
	return (*eof_reached);
}

void	read_from_stdin(int pipe_fd[2], char *delim)
{
	bool	eof_reached;

	close(pipe_fd[0]);
	eof_reached = false;
	g_signal_received = 0;
	disable_echoctl();
	heredoc_loop(pipe_fd, delim, &eof_reached);
	restore_terminal();
	if (eof_reached)
		handle_eof_heredoc(delim);
	cleanup_and_exit(pipe_fd[1]);
}
