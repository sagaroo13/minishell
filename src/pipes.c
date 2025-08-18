/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:59:23 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/08 14:59:23 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	run_child(t_command *cmd, t_shell *shell, int in_fd, int out_fd)
{
	int	ret;

	set_signals(MODE_CHILD);
	if (in_fd != STDIN_FILENO)
	{
		safe_dup2(in_fd, STDIN_FILENO);
		safe_close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		safe_dup2(out_fd, STDOUT_FILENO);
		safe_close(out_fd);
	}
	redirs(cmd);
	if (cmd->builtin)
	{
		ret = exec_builtin(cmd->args, shell);
		update_last_exit_status(shell, ret);
		exit(ret);
	}
	exec(cmd->args[0], cmd->args, shell);
	update_last_exit_status(shell, 1);
	exit(EXIT_FAILURE);
}

void	exec_pipe(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		run_child(cmd, shell, STDIN_FILENO, pipe_fd[1]);
	else
	{
		safe_close(pipe_fd[1]);
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
		update_last_exit_status(shell, status);
		safe_dup2(pipe_fd[0], STDIN_FILENO);
		safe_close(pipe_fd[0]);
	}
}

void	exec_last(t_command *cmd, t_shell *shell)
{
	int		ret;
	pid_t	pid;
	int		status;

	if (cmd->builtin)
	{
		redirs(cmd);
		ret = exec_builtin(cmd->args, shell);
		update_last_exit_status(shell, ret);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
		run_child(cmd, shell, STDIN_FILENO, STDOUT_FILENO);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
		update_last_exit_status(shell, status);
	}
}
