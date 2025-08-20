/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:21:36 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 01:38:26 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		free_env(shell->env);
}

static void	free_partial_env(char **env, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(env[j]);
		j++;
	}
	free(env);
}

char	**copy_envp(char **envp)
{
	int		n;
	char	**new_env;
	int		i;

	n = 0;
	i = 0;
	while (envp && envp[n])
		n++;
	new_env = malloc(sizeof(char *) * (n + 1));
	if (!new_env)
		return (NULL);
	while (i < n)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			free_partial_env(new_env, i);
			return (NULL);
		}
		i++;
	}
	new_env[n] = NULL;
	return (new_env);
}

char	*get_path(char *line, t_shell *shell)
{
	char	*env_path;
	char	**paths;
	char	*executable_path;

	env_path = get_env_value(shell, "PATH");
	if (!env_path || !*env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	executable_path = try_executable_path(paths, line);
	ft_free_matrix(paths);
	return (executable_path);
}

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
	return (NULL);
}