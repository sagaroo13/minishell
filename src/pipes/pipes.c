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

bool	do_builtin(t_command *cmd, t_shell *shell)
{
	int	ret;

	if (cmd->builtin)
	{
		if (redirs(cmd) != 0)
		{
			set_exit_status_direct(shell, 1);
			return (false);
		}
		ret = exec_builtin(cmd->args, shell);
		set_exit_status_direct(shell, ret);
		return (false);
	}
	return (true);
}

void	exec_last(t_command *cmd, t_shell *shell)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0])
	{
		if (redirs(cmd) != 0)
			set_exit_status_direct(shell, 1);
		else
			set_exit_status_direct(shell, 0);
		return ;
	}
	if (!do_builtin(cmd, shell))
		return ;
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
	{
		if (is_interactive_command(cmd->args[0]))
			child_exec_interactive_pipe(cmd, shell, pipe_fd);
		else
			child_exec_pipe(cmd, shell, pipe_fd);
	}
	else
		parent_setup_pipe_and_wait(shell, pipe_fd, pid);
}
