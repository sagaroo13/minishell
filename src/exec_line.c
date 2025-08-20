/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:31:25 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 00:40:39 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char *find_path(char **envp)
// {
// 	char *env_path;
// 	int i;

// 	env_path = NULL;
// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
// 		{
// 			env_path = envp[i] + 5;
// 			break ;
// 		}
// 		i++;
// 	}
// 	return (env_path);
// }

char	*get_env_value(t_shell *shell, const char *name)
{
	int		i;
	char	*eq;

	i = 0;
	if (!shell || !name)
		return (NULL);
	while (shell->env && shell->env[i])
	{
		eq = ft_strchr(shell->env[i], '=');
		if (eq && ft_strncmp(shell->env[i], name, eq - shell->env[i]) == 0
			&& (size_t)(eq - shell->env[i]) == ft_strlen(name))
			return (eq + 1);
		i++;
	}
	return (NULL);
}

static void	handle_command_not_found(char *cmd_name, t_shell *shell)
{
	if (!ft_strchr(cmd_name, '/'))
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd_name, 2);
	}
	else
	{
		ft_putstr_fd("minishell: No such file or directory: ", 2);
		ft_putendl_fd(cmd_name, 2);
	}
	update_last_exit_status(shell, 127);
}

void	exec(char *cmd_name, char **cmd_args, t_shell *shell)
{
	int		builtin_status;
	char	*path;

	expand_exit_status(cmd_args, shell);
	if (is_builtin(cmd_name))
	{
		builtin_status = exec_builtin(cmd_args, shell);
		update_last_exit_status(shell, builtin_status);
		return ;
	}
	path = get_path(cmd_name, shell);
	if (!path)
	{
		handle_command_not_found(cmd_name, shell);
		return ;
	}
	if (execve(path, cmd_args, shell->env) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd_name);
		update_last_exit_status(shell, 127);
	}
	free(path);
}

static void	process_heredoc_and_exec(t_command_line *cmd_line,
		t_shell *shell)
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

void	exec_line(char *line, t_shell *shell)
{
	t_command_line	cmd_line;

	parse_line(&cmd_line, shell, line);
	if (!cmd_line.execute)
	{
		if (cmd_line.err_msg)
			perror(cmd_line.err_msg);
		free_cmd_line(&cmd_line);
		return ;
	}
	process_heredoc_and_exec(&cmd_line, shell);
	if (cmd_line.execute)
		free_cmd_line(&cmd_line);
}
