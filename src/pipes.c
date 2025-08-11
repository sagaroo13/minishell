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

static void	run_child(t_command *cmd, char **envp, int in_fd, int out_fd)
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
		ret = exec_builtin(cmd->args, envp);
		update_last_exit_status(&g_last_exit_status, ret);
		exit(ret);
	}
	exec(cmd->args[0], cmd->args, envp);
	update_last_exit_status(&g_last_exit_status, 1);
	exit(EXIT_FAILURE);
}

static void	run_parent(pid_t pid, int close_fd, int restore_signal)
{
	int	status;

	if (close_fd != -1)
		safe_close(close_fd);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	update_last_exit_status(&g_last_exit_status, status);
	if (restore_signal)
	{
		signal(SIGINT, sigint_handler);
		set_signals(MODE_SHELL);
	}
}

void	exec_pipe(t_command *cmd, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		run_child(cmd, envp, STDIN_FILENO, pipe_fd[1]);
	else
	{
		run_parent(pid, pipe_fd[1], 0);
		safe_dup2(pipe_fd[0], STDIN_FILENO);
		safe_close(pipe_fd[0]);
	}
}

void	exec_last(t_command *cmd, char **envp)
{
	int		ret;
	pid_t	pid;

	if (cmd->builtin)
	{
		redirs(cmd);
		ret = exec_builtin(cmd->args, envp);
		update_last_exit_status(&g_last_exit_status, ret);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		update_last_exit_status(&g_last_exit_status, 1);
		return ;
	}
	if (pid == 0)
		run_child(cmd, envp, STDIN_FILENO, STDOUT_FILENO);
	else
		run_parent(pid, -1, 1);
}
