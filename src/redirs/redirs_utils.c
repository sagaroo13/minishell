/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:15:53 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/23 17:33:52 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	search_last_redir(t_redirections red, char *cmd_str, int *iter)
{
	int		i;
	char	*p;
	int		pos;

	i = -1;
	*iter = 0;
	if (!cmd_str || !red.redirs)
		return ;
	while (++i < red.n_redirs)
	{
		if (!red.redirs[i])
			continue ;
		p = ft_strstr(cmd_str, red.redirs[i]);
		if (p)
		{
			pos = p - cmd_str;
			if (pos > *iter)  // Solo actualiza si la posición es mayor
				*iter = pos;
		}
	}
}

void	open_all_files(t_redirections red, t_open_flags flags)
{
	int	i;
	int	fd;

	i = -1;
	if (!red.redirs)
		return ;
	while (++i < red.n_redirs)
	{
		if (!red.redirs[i])
			continue ;
		fd = safe_open(red.redirs[i], flags);
		safe_close(fd);
	}
}

static int	handle_stdout_redir(t_command *cmd)
{
	int	fd;

	if (!cmd->stdout.redirs || cmd->stdout.n_redirs == 0
		|| !cmd->stdout.redirs[cmd->stdout.n_redirs - 1])
		return (1);
	fd = safe_open(cmd->stdout.redirs[cmd->stdout.n_redirs - 1], WRITE);
	if (fd == -1)
		return (1);
	safe_dup2(fd, STDOUT_FILENO);
	safe_close(fd);
	return (0);
}

static int	handle_append_redir(t_command *cmd)
{
	int	fd;

	if (!cmd->append.redirs || cmd->append.n_redirs == 0
		|| !cmd->append.redirs[cmd->append.n_redirs - 1])
		return (1);
	fd = safe_open(cmd->append.redirs[cmd->append.n_redirs - 1], APPEND);
	if (fd == -1)
		return (1);
	safe_dup2(fd, STDOUT_FILENO);
	safe_close(fd);
	return (0);
}

int	redir_out(t_command *cmd)
{
	int	i;
	int	j;

	open_all_files(cmd->stdout, WRITE);
	open_all_files(cmd->append, APPEND);
	search_last_redir(cmd->stdout, cmd->cmd_str, &i);
	search_last_redir(cmd->append, cmd->cmd_str, &j);
	if (cmd->stdout.n_redirs == 0 && cmd->append.n_redirs == 0)
		return (0);
	else if (i > j)
		return (handle_stdout_redir(cmd));
	else
		return (handle_append_redir(cmd));
}
