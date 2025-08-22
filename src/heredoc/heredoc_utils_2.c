/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:04:39 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 18:05:22 by shirakim         ###   ########.fr       */
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

static void	handle_eof_heredoc(char *delim)
{
	ft_putstr_fd("\nminishell: warning: here-document "
		"delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delim, 2);
	ft_putstr_fd("')\n", 2);
	exit(1);
}

static void	cleanup_and_exit(int pipe_fd)
{
	close(pipe_fd);
	get_next_line(-1);
	exit(EXIT_SUCCESS);
}

void	read_from_stdin(int pipe_fd[2], char *delim)
{
	char			*line;
	bool			eof_reached;
	bool			is_interactive;
	struct termios	term;

	close(pipe_fd[0]);
	eof_reached = false;
	is_interactive = (tcgetattr(STDIN_FILENO, &term) == 0);  // Detectar si es terminal o pipe
	
	while (1)
	{
		if (is_interactive)  // Solo mostrar prompt si es interactivo
			write(STDOUT_FILENO, "heredoc> ", 9);
			
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			eof_reached = true;
			break ;
		}
		if (!process_heredoc_line(line, delim, pipe_fd))
			break ;
	}
	if (eof_reached)
		handle_eof_heredoc(delim);
	cleanup_and_exit(pipe_fd[1]);
}
