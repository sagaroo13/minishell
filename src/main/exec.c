/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:31:25 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 12:38:39 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_heredoc_and_exec(t_command_line *cmd_line, t_shell *shell)
{
	count_cat_commands(cmd_line, shell);
	if (is_cat_pipeline_pattern(cmd_line))
		execute_cat_pipeline(cmd_line, shell);
	else
		execute_normal_pipeline(cmd_line, shell);
}

void	exec_line(char *line, t_shell *shell)
{
	t_command_line	cmd_line;
	int i;

	parse_line(&cmd_line, shell, line);
	shell->cmd_line = &cmd_line;

	// Expand variables after parsing but before execution
	for (i = 0; i < cmd_line.n_cmds; i++)
	{
		if (cmd_line.cmds[i].args)
		{
			expand_exit_status(cmd_line.cmds[i].args, shell);
		}
	}

	if (!cmd_line.execute)
	{
		if (cmd_line.err_msg)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_line.err_msg, 2);
			ft_putstr_fd("\n", 2);
			if (ft_strstr(cmd_line.err_msg, "syntax error near"
					"unexpected token"))
				set_exit_status_direct(shell, 1);
			else
				set_exit_status_direct(shell, 2);
		}
		free_cmd_line(&cmd_line);
		return ;
	}
	process_heredoc_and_exec(&cmd_line, shell);
	free_cmd_line(&cmd_line);
}
