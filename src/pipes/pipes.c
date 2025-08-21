/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:59:23 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 21:00:00 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// helpers are implemented in pipes_utils.c

void	exec_last(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		ret;

	if (cmd->builtin)
	{
		ret = exec_builtin(cmd->args, shell);
		update_last_exit_status(shell, ret);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("fork");
		return ;
	}
	if (pid == 0)
		child_exec_command(cmd, shell);
	parent_wait_and_finalize(shell, pid);
}

void	exec_pipe(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		child_exec_pipe(cmd, shell, pipe_fd);
	else
		parent_setup_pipe_and_wait(shell, pipe_fd, pid);
}
