/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:13:44 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/23 20:04:48 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_cat_command(t_command *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "cat") == 0
		|| ft_strcmp(cmd->args[0], "/bin/cat") == 0)
		return (1);
	return (0);
}

int	is_cat_pipeline_pattern(t_command_line *cmd_line)
{
	int	i;

	i = 0;
	if (cmd_line->n_cmds < 2)
		return (0);
	while (i < cmd_line->n_cmds - 1)
	{
		if (!is_cat_command(&cmd_line->cmds[i]))
			return (0);
		i++;
	}
	if (cmd_line->cmds[cmd_line->n_cmds - 1].args
		&& cmd_line->cmds[cmd_line->n_cmds - 1].args[0]
		&& ft_strcmp(cmd_line->cmds[cmd_line->n_cmds - 1].args[0],
			"cat") != 0
		&& ft_strcmp(cmd_line->cmds[cmd_line->n_cmds - 1].args[0],
			"/bin/cat") != 0)
		return (1);
	return (0);
}

void	execute_cat_pipeline(t_command_line *cmd_line, t_shell *shell)
{
	int	i;

	i = 0;
	exec_last(&cmd_line->cmds[cmd_line->n_cmds - 1], shell);
	while (i < cmd_line->n_cmds - 1)
	{
		if (cmd_line->cmds[i].heredoc.redirs)
		{
			set_signals(MODE_HEREDOC);
			heredoc(&cmd_line->cmds[i]);
			if (!cmd_line->execute)
				return ;
		}
		if (i != cmd_line->n_cmds - 2)
			exec_pipe(&cmd_line->cmds[i], shell);
		else
			exec_last(&cmd_line->cmds[i], shell);
		i++;
	}
}

void	execute_normal_pipeline(t_command_line *cmd_line, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < cmd_line->n_cmds)
	{
		if (cmd_line->cmds[i].heredoc.redirs)
		{
			set_signals(MODE_HEREDOC);
			heredoc(&cmd_line->cmds[i]);
			if (!cmd_line->execute)
				return ;
		}
		if (i != cmd_line->n_cmds - 1)
			exec_pipe(&cmd_line->cmds[i], shell);
		else
			exec_last(&cmd_line->cmds[i], shell);
		i++;
	}
}

void	count_cat_commands(t_command_line *cmd_line, t_shell *shell)
{
	int	i;
	int	cat_count;

	cat_count = 0;
	i = 0;
	while (i < cmd_line->n_cmds)
	{
		if (cmd_line->cmds[i].args && cmd_line->cmds[i].args[0]
			&& (ft_strcmp(cmd_line->cmds[i].args[0], "cat") == 0
				|| ft_strcmp(cmd_line->cmds[i].args[0], "/bin/cat") == 0))
			cat_count++;
		i++;
	}
	shell->cat_count = cat_count;
}
