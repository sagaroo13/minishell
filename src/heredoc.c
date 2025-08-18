/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:30:14 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/18 23:32:17 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void sigint_handler_in_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);  // imprime el salto de línea inmediatamente
   // get_next_line(-1); // limpiar buffers internos
    exit(130);
}


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

static int	run_single_heredoc(char *delim, int is_last)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0) // hijo
	{
		signal(SIGINT, sigint_handler_in_heredoc);
		read_from_stdin(pipe_fd, delim);
	}
	else // padre
	{
		signal(SIGINT, SIG_IGN);
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		set_signals(MODE_SHELL);
		if (is_last)
			return (pipe_fd[0]);
		close(pipe_fd[0]);
	}
	return (-1);
}

static int	process_all_heredocs(t_command *cmd)
{
	int	i;
	int	last_fd = -1;

	i = 0;
	while (i < cmd->heredoc.n_redirs)
	{
		last_fd = run_single_heredoc(
			cmd->heredoc.redirs[i],
			(i == cmd->heredoc.n_redirs - 1)
		);
		i++;
	}
	return (last_fd);
}

void	heredoc(t_command *cmd)
{
	int	last_fd;

	if (cmd->heredoc.n_redirs > 16) // límite arbitrario por seguridad
	{
		perror("syntax error: too many heredocs");
		return ;
	}
	last_fd = process_all_heredocs(cmd);
	if (last_fd != -1)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
}
