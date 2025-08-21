/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:30:14 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/21 15:15:22 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	heredoc(t_command *cmd)
{
	int		i;
	int		pipe_fd[2];
	int		last_fd;
	pid_t	pid;
	t_heredoc_ctx		ctx;

	last_fd = -1;
	i = 0;
	if (cmd->heredoc.n_redirs < 1)
		return ;
	while (i < cmd->heredoc.n_redirs)
	{
		pid = start_heredoc_child(pipe_fd, cmd->heredoc.redirs[i]);
		if (pid == -1)
			exit(EXIT_FAILURE);
		ctx.pipe_fd[0] = pipe_fd[0];
		ctx.pipe_fd[1] = pipe_fd[1];
		ctx.plast_fd = &last_fd;
		ctx.is_last = (i == cmd->heredoc.n_redirs - 1);
		if (!handle_parent_after_child(cmd, pid, &ctx))
			return ;
		i++;
	}
	attach_last_heredoc_to_stdin(last_fd);
	set_signals(MODE_SHELL);
}
