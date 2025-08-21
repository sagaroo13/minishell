/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:21:36 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 14:54:27 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Nos podemos ahorrar esta función, ft_free_matrix es segura con condicion if
// static void	free_partial_env(char **env, int count)
// {
// 	int	j;

// 	j = 0;
// 	while (j < count)
// 	{
// 		free(env[j]);
// 		j++;
// 	}
// 	free(env);
// }

char	**copy_envp(char **envp)
{
	int		n;
	int		i;
	char	**new_env;

	n = 0;
	i = -1;
	while (envp && envp[n])
		n++;
	new_env = safe_malloc(sizeof(char *) * (n + 1), false);
	while (++i < n)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			ft_free_matrix(new_env);
			return (NULL);
		}
	}
	new_env[n] = NULL;
	return (new_env);
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
