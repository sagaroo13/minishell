/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 09:55:42 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 12:38:41 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

	if (!cmd_args || !*cmd_args)
		return;

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

bool	is_interactive_command(const char *cmd_name)
{
	if (!cmd_name)
		return (false);
	return (ft_strcmp((char *)cmd_name, "cat") == 0
		|| ft_strcmp((char *)cmd_name, "/bin/cat") == 0);
}
