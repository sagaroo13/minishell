/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:12:15 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/08 15:12:15 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	redir_in(t_command *cmd)
{
	int	fd;

	if (cmd->stdin.redirs)
	{
		fd = safe_open(cmd->stdin.redirs[cmd->stdin.n_redirs - 1], READ);
		if (fd == -1)
			return (true);
		safe_dup2(fd, STDIN_FILENO);
		safe_close(fd);
	}
	return (false);
}

bool	redir_out(t_command *cmd)
{
	int	fd;
	int	i;
	int	j;

	if (cmd->stdout.n_redirs == 0 && cmd->append.n_redirs == 0)
		return (false);
	open_all_files(cmd->stdout, WRITE);
	open_all_files(cmd->append, APPEND);
	fd = -1;
	i = cmd->stdout.last_index;
	j = cmd->append.last_index;
	if (i > j)
		fd = safe_open(cmd->stdout.redirs[cmd->stdout.n_redirs - 1], WRITE);
	else
		fd = safe_open(cmd->append.redirs[cmd->append.n_redirs - 1], APPEND);
	if (fd >= 0)
	{
		safe_dup2(fd, STDOUT_FILENO);
		safe_close(fd);
	}
	else
		return (true);
	return (false);
}

bool	redir_err(t_command *cmd)
{
	int	fd;

	if (cmd->stderr.n_redirs == 0)
		return (false);
	open_all_files(cmd->stderr, WRITE);
	if (cmd->stderr.redirs && cmd->stderr.n_redirs > 0
		&& cmd->stderr.redirs[cmd->stderr.n_redirs - 1])
	{
		fd = safe_open(cmd->stderr.redirs[cmd->stderr.n_redirs - 1], WRITE);
		if (fd == -1)
			return (true);
		safe_dup2(fd, STDERR_FILENO);
		safe_close(fd);
	}
	return (false);
}

bool	redirs(t_command *cmd)
{
	if (redir_in(cmd))
		return (true);
	if (redir_out(cmd))
		return (true);
	if (redir_err(cmd))
		return (true);
	return (false);
}
