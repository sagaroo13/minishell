/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:46:54 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 20:44:40 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	find_env_index(const char *name, char **env)
{
	int		i;
	char	*eq;
	size_t	key_len;

	i = 0;
	if (!name || !env)
		return (-1);
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
			key_len = (size_t)(eq - env[i]);
		else
			key_len = ft_strlen(env[i]);
		if (ft_strlen(name) == key_len
			&& ft_strncmp(env[i], name, key_len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	remove_env_var(const char *name, char **env)
{
	int	i;
	int	j;

	i = find_env_index(name, env);
	if (i == -1)
		return (1);
	free(env[i]);
	j = i;
	while (env[j])
	{
		env[j] = env[j + 1];
		j++;
	}
	return (0);
}

static int	unset_one_var(char *arg, t_shell *shell)
{
	if (ft_strchr(arg, '='))
	{
		ft_putstr_fd("minishell: unset: invalid name: ", 2);
		ft_putendl_fd(arg, 2);
		return (1);
	}
	if (remove_env_var(arg, shell->env))
	{
		ft_putstr_fd("minishell: unset: variable not found: ", 2);
		ft_putendl_fd(arg, 2);
		return (1);
	}
	return (0);
}

int	env_unset(char **argv, t_shell *shell)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	if (!argv || !argv[0] || !shell || !shell->env)
		return (1);
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		if (unset_one_var(argv[i], shell))
			status = 1;
		i++;
	}
	shell->last_status.last_exit_code = status;
	shell->last_status.status = status;
	shell->last_status.exit_called = false;
	return (status);
}
