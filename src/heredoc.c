/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:30:14 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/18 23:05:34 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include "../include/minishell.h"

static void	read_from_stdin(int pipe_fd[2], char *delim)
{
	char	*line;
	char	*new_line;

	close(pipe_fd[0]); // hijo no lee, solo escribe
	write(1, "heredoc> ", 9);
	line = get_next_line(0);
	while (line)
	{
		new_line = ft_strchr(line, '\n');
		if (new_line)
			*new_line = '\0';
		if (!ft_strcmp(delim, line))
		{
			free(line);
			get_next_line(-1); // limpiar buffer de GNL
			close(pipe_fd[1]);
			exit(EXIT_SUCCESS);
		}
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
	}
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

void	heredoc(t_command *cmd)
{
	int		i;
	int		pipe_fd[2];
	int		last_fd = -1;
	pid_t	pid;

	if (cmd->heredoc.n_redirs > 16) // límite arbitrario por seguridad
	{
		perror("syntax error: too many heredocs");
		return ;
	}
	i = 0;
	while (i < cmd->heredoc.n_redirs)
	{
		if (pipe(pipe_fd) == -1)
			exit(EXIT_FAILURE);
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0) // hijo
		{
			set_signals(MODE_CHILD);
			read_from_stdin(pipe_fd, cmd->heredoc.redirs[i]);
		}
		else // padre
		{
			int	status;

			signal(SIGINT, SIG_IGN);
			close(pipe_fd[1]);        // padre no escribe
			waitpid(pid, &status, 0); // esperar hijo
			if (i == cmd->heredoc.n_redirs - 1)
				last_fd = pipe_fd[0]; // último heredoc → conservar fd
			else
				close(pipe_fd[0]);    // descartar fd intermedio
		}
		i++;
	}
	if (last_fd != -1)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
}
