/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:26:09 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/23 20:32:37 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	setup_interactive_pipe(t_command *cmd, t_shell *shell,
	int pipe_fd[2])
{
	set_signals(MODE_CHILD);
	safe_close(pipe_fd[0]);
	safe_dup2(pipe_fd[1], STDOUT_FILENO);
	safe_close(pipe_fd[1]);
	if (redirs(cmd) != 0)
		exit(1);
	if (cmd->stdin.n_redirs > 0 || cmd->stdout.n_redirs > 0
		|| cmd->append.n_redirs > 0 || cmd->stderr.n_redirs > 0
		|| cmd->heredoc.n_redirs > 0)
	{
		exec(cmd->args[0], cmd->args, shell);
		if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
			exit(126);
		else
			exit(127);
		return (1);
	}
	return (0);
}

static void	process_interactive_input(int needed_newlines)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	int		consecutive_newlines;

	consecutive_newlines = 0;
	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
		if (bytes_read <= 0)
			exit(0);
		if (bytes_read == 1 && buffer[0] == '\n')
		{
			consecutive_newlines++;
			write(STDOUT_FILENO, buffer, bytes_read);
			if (consecutive_newlines >= needed_newlines)
				exit(0);
		}
		else
		{
			consecutive_newlines = 0;
			write(STDOUT_FILENO, buffer, bytes_read);
		}
	}
}

void	child_exec_interactive_pipe(t_command *cmd, t_shell *shell,
	int pipe_fd[2])
{
	int	needed_newlines;

	needed_newlines = 1;
	if (shell->cat_count > 0)
		needed_newlines = shell->cat_count;
	if (setup_interactive_pipe(cmd, shell, pipe_fd))
		return ;
	process_interactive_input(needed_newlines);
}
