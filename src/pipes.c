/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:03:17 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/07/08 17:03:17 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_pipe(t_command *cmd, char **envp)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (!pid)
	{
		set_signals(MODE_PIPE);
		safe_close(pipe_fd[0]);
		safe_dup2(pipe_fd[1], STDOUT_FILENO);
		safe_close(pipe_fd[1]);
		redirs(cmd);
		exec(cmd->args[0], cmd->args, envp);
		update_last_exit_status(&g_last_exit_status, 1);
		exit(EXIT_FAILURE); // Si exec falla, salir con error
	}
	else
	{
		signal(SIGINT, SIG_IGN); // Ignorar SIGINT en el padre mientras espera
		safe_close(pipe_fd[1]);
		safe_dup2(pipe_fd[0], STDIN_FILENO);
		safe_close(pipe_fd[0]);
		waitpid(pid, &status, 0);
		update_last_exit_status(&g_last_exit_status, status);
	}
}

void	exec_last(t_command *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	if (cmd->builtin)
	{
		int ret = exec_builtin(cmd->args, envp);
		update_last_exit_status(&g_last_exit_status, ret);
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		update_last_exit_status(&g_last_exit_status, 1);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		set_signals(MODE_CHILD);
		redirs(cmd);
		set_exit_status_direct(1);
		exec(cmd->args[0], cmd->args, envp);
		update_last_exit_status(&g_last_exit_status, 1);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, sigint_handler); // Restaurar tu handler después
		update_last_exit_status(&g_last_exit_status, status);
	}
}

