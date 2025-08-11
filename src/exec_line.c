/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:31:25 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/05 23:36:44 by shirakim         ###   ########.fr       */
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

char	*try_executable_path(char **paths, char *line)
{
	char	*path_part;
	char	*path;
	int		i;

	i = -1;
	while (paths[++i])
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

char	*get_path(char *line)
{
	char	*env_path;
	char	**paths;
	char	*executable_path;

	env_path = getenv("PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	executable_path = try_executable_path(paths, line);
	ft_free_matrix(paths);
	return (executable_path);
}

void	exec(char *cmd_name, char **cmd_args, char **envp)
{
	int		builtin_status;
	char	*path;

	expand_exit_status(cmd_args, &g_last_exit_status);
	if (is_builtin(cmd_name))
	{
		builtin_status = exec_builtin(cmd_args, envp);
		update_last_exit_status(&g_last_exit_status, builtin_status);
		return ;
	}
	path = get_path(cmd_name);
	if (!path)
	{
		perror("command not found");
		exit(127);
	}
	if (execve(path, cmd_args, envp) == -1)
	{
		perror("execve");
		exit(127);
	}
	free(path);
}

void	exec_line(char *line, char **envp)
{
	t_command_line	cmd_line;
	int				i;

	parse_line(&cmd_line, line);
	if (!cmd_line.execute)
	{
		perror(cmd_line.err_msg);
		free_cmd_line(&cmd_line);
		return ;
	}
	i = -1;
	while (++i < cmd_line.n_cmds)
	{
		if (cmd_line.cmds[i].heredoc.redirs)
			heredoc(&cmd_line.cmds[i]);
		if (i != cmd_line.n_cmds - 1)
			exec_pipe(&cmd_line.cmds[i], envp);
		else
			exec_last(&cmd_line.cmds[i], envp);
	}
	free_cmd_line(&cmd_line);
}
