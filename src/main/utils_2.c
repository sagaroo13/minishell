/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:09:35 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 20:40:05 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env(t_shell *shell, const char *name)
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
