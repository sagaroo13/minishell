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
		safe_close(pipe_fd[0]);
		safe_dup2(pipe_fd[1], STDOUT_FILENO);
		safe_close(pipe_fd[1]);
		redirs(cmd);
		exec(cmd->args[0], cmd->args, envp);
	}
	else
	{
		safe_close(pipe_fd[1]);
		safe_dup2(pipe_fd[0], STDIN_FILENO);
		safe_close(pipe_fd[0]);
		waitpid(pid, &status, 0);
	}
}

void	exec_last(t_command *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	if (cmd->builtin)
	{
		exec_builtin(cmd->args, envp);
		return ;
	}
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (!pid)
	{
		redirs(cmd);
		exec(cmd->args[0], cmd->args, envp);
	}
	else
		waitpid(pid, &status, 0);
}
