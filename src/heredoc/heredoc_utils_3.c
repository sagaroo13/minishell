/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 10:05:15 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 10:55:24 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_eof_heredoc(char *delim)
{
	get_next_line(-1);
	ft_putstr_fd("minishell: warning: here-document "
		"delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delim, 2);
	ft_putstr_fd("')\n", 2);
	exit(1);
}

void	cleanup_and_exit(int pipe_fd)
{
	close(pipe_fd);
	get_next_line(-1);
	exit(EXIT_SUCCESS);
}

bool	process_heredoc_line(char *line, char *delim, int pipe_fd[2])
{
	if (!line)
		return (false);
	if (!ft_strcmp(delim, line))
	{
		free(line);
		return (false);
	}
	ft_putendl_fd(line, pipe_fd[1]);
	free(line);
	return (true);
}
