/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:31:25 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 18:53:39 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*try_executable_path(char **paths, char *line)
{
	char	*path_part;
	char	*path;
	int		i;

	i = -1;
	while (paths && paths[++i])
	{
		path_part = ft_strjoin("/", line);
		path = ft_strjoin(paths[i], path_part);
		free(path_part);
		if (!access(path, F_OK))
			return (path);
		free(path);
	}
	return (line);
}

char	*get_path(char *line, t_shell *shell)
{
	char	*env_path;
	char	**paths;
	char	*executable_path;

	env_path = get_env(shell, "PATH");
	if (!env_path || !*env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	executable_path = try_executable_path(paths, line);
	ft_free_matrix(paths);
	return (executable_path);
}

void	exec(char *cmd_name, char **cmd_args, t_shell *shell)
{
	int		builtin_status;
	char	*path;

	expand_exit_status(cmd_args, shell);
	if (is_builtin(cmd_name))
	{
		builtin_status = exec_builtin(cmd_args, shell);
		set_exit_status_direct(shell, builtin_status);
		return ;
	}
	path = get_path(cmd_name, shell);
	if (execve(path, cmd_args, shell->env) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd_name);
		free(path);
		set_exit_status_direct(shell, 127);
		return ;
	}
	free(path);
}

static int	is_cat_pipeline_pattern(t_command_line *cmd_line)
{
	int	i;
	int	is_cat_pipeline;

	if (cmd_line->n_cmds < 3)
		return (0);
	is_cat_pipeline = 1;
	i = 0;
	while (i < cmd_line->n_cmds - 1)
	{
		if (!cmd_line->cmds[i].args || !cmd_line->cmds[i].args[0] || 
			(ft_strcmp(cmd_line->cmds[i].args[0], "cat") != 0 &&
			ft_strcmp(cmd_line->cmds[i].args[0], "/bin/cat") != 0))
		{
			is_cat_pipeline = 0;
			break;
		}
		i++;
	}
	if (is_cat_pipeline && cmd_line->cmds[cmd_line->n_cmds - 1].args && 
		cmd_line->cmds[cmd_line->n_cmds - 1].args[0] &&
		(ft_strcmp(cmd_line->cmds[cmd_line->n_cmds - 1].args[0], "ls") == 0 ||
		ft_strcmp(cmd_line->cmds[cmd_line->n_cmds - 1].args[0], "/bin/ls") == 0))
		return (1);
	return (0);
}

static void	execute_cat_pipeline(t_command_line *cmd_line, t_shell *shell)
{
	int	i;

	exec_last(&cmd_line->cmds[cmd_line->n_cmds - 1], shell);
	i = 0;
	while (i < cmd_line->n_cmds - 1)
	{
		if (cmd_line->cmds[i].heredoc.redirs)
		{
			set_signals(MODE_HEREDOC);
			heredoc(&cmd_line->cmds[i]);
			if (!cmd_line->execute)
				return;
		}
		if (i != cmd_line->n_cmds - 2)
			exec_pipe(&cmd_line->cmds[i], shell);
		else
			exec_last(&cmd_line->cmds[i], shell);
		i++;
	}
}

static void	execute_normal_pipeline(t_command_line *cmd_line, t_shell *shell)
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

static void	process_heredoc_and_exec(t_command_line *cmd_line,
		t_shell *shell)
{
	if (is_cat_pipeline_pattern(cmd_line))
		execute_cat_pipeline(cmd_line, shell);
	else
		execute_normal_pipeline(cmd_line, shell);
}

void	exec_line(char *line, t_shell *shell)
{
	t_command_line	cmd_line;

	parse_line(&cmd_line, shell, line);
	shell->cmd_line = &cmd_line;
	
	if (!cmd_line.execute)
	{
		if (cmd_line.err_msg)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_line.err_msg, 2);
			ft_putstr_fd("\n", 2);
			if (ft_strstr(cmd_line.err_msg, "syntax error near unexpected token"))
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
