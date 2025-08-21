/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:29:04 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 18:05:36 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	normalize_wait_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}

void	child_exec_command(t_command *cmd, t_shell *shell)
{
	set_signals(MODE_CHILD);
	redirs(cmd);
	exec(cmd->args[0], cmd->args, shell);
	if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
		exit(126);
	else
		exit(127);
}

void	parent_wait_and_finalize(t_shell *shell, pid_t pid)
{
	int	status;

	set_signals(MODE_PIPE);
	waitpid(pid, &status, 0);
	update_last_exit_status(shell, normalize_wait_status(status));
	set_signals(MODE_SHELL);
}

void	child_exec_pipe(t_command *cmd, t_shell *shell, int pipe_fd[2])
{
	set_signals(MODE_CHILD);
	safe_close(pipe_fd[0]);
	safe_dup2(pipe_fd[1], STDOUT_FILENO);
	safe_close(pipe_fd[1]);
	redirs(cmd);
	exec(cmd->args[0], cmd->args, shell);
	if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
		exit(126);
	else
		exit(127);
}

void	parent_setup_pipe_and_wait(t_shell *shell, int pipe_fd[2], pid_t pid)
{
	int	status;

	set_signals(MODE_PIPE);
	safe_close(pipe_fd[1]);
	safe_dup2(pipe_fd[0], STDIN_FILENO);
	safe_close(pipe_fd[0]);
	waitpid(pid, &status, 0);
	update_last_exit_status(shell, normalize_wait_status(status));
	set_signals(MODE_SHELL);
}
