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

int	redir_in(t_command *cmd)
{
	int	fd;

	if (cmd->stdin.redirs)
	{
		fd = safe_open(cmd->stdin.redirs[cmd->stdin.n_redirs - 1], READ);
		if (fd == -1)
			return (1); // Error al abrir el archivo
		safe_dup2(fd, STDIN_FILENO);
		safe_close(fd);
	}
	return (0); // Éxito
}

int	redir_out(t_command *cmd)
{
	int	fd;
	int	i;
	int	j;

	open_all_files(cmd->stdout, WRITE);
	open_all_files(cmd->append, APPEND);
	search_last_redir(cmd->stdout, cmd->cmd_str, &i);
	search_last_redir(cmd->append, cmd->cmd_str, &j);
	if (cmd->stdout.n_redirs == 0 && cmd->append.n_redirs == 0)
		return (0);
	else if (i > j)
	{
		if (!cmd->stdout.redirs || cmd->stdout.n_redirs == 0 || 
			!cmd->stdout.redirs[cmd->stdout.n_redirs - 1])
			return (1);
		fd = safe_open(cmd->stdout.redirs[cmd->stdout.n_redirs - 1], WRITE);
		if (fd == -1)
			return (1); // Error al abrir el archivo
		safe_dup2(fd, STDOUT_FILENO);
		safe_close(fd);
	}
	else
	{
		if (!cmd->append.redirs || cmd->append.n_redirs == 0 || 
			!cmd->append.redirs[cmd->append.n_redirs - 1])
			return (1);
		fd = safe_open(cmd->append.redirs[cmd->append.n_redirs - 1], APPEND);
		if (fd == -1)
			return (1); // Error al abrir el archivo
		safe_dup2(fd, STDOUT_FILENO);
		safe_close(fd);
	}
	return (0); // Éxito
}

int	redir_err(t_command *cmd)
{
	int	fd;

	open_all_files(cmd->stderr, WRITE);
	if (cmd->stderr.redirs && cmd->stderr.n_redirs > 0 && 
		cmd->stderr.redirs[cmd->stderr.n_redirs - 1])
	{
		fd = safe_open(cmd->stderr.redirs[cmd->stderr.n_redirs - 1], WRITE);
		if (fd == -1)
			return (1); // Error al abrir el archivo
		safe_dup2(fd, STDERR_FILENO);
		safe_close(fd);
	}
	return (0); // Éxito
}

int	redirs(t_command *cmd)
{
	if (redir_in(cmd) != 0)
		return (1);
	if (redir_out(cmd) != 0)
		return (1);
	if (redir_err(cmd) != 0)
		return (1);
	return (0);
}
