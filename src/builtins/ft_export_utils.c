/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:19:07 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 19:54:10 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_env_var(char *name, char *new_var, t_shell *shell)
{
	int	j;

	j = 0;
	while (shell->env[j])
	{
		if (ft_strncmp(shell->env[j], name, ft_strlen(name)) == 0
			&& (shell->env[j][ft_strlen(name)] == '='
			|| shell->env[j][ft_strlen(name)] == '\0'))
		{
			free(shell->env[j]);
			shell->env[j] = new_var;
			return (1);
		}
		j++;
	}
	return (0);
}

void	add_env_var(char *new_var, t_shell *shell)
{
	int		j;
	int		k;
	char	**new_env;

	j = 0;
	k = 0;
	while (shell->env[j])
		j++;
	new_env = malloc(sizeof(char *) * (j + 2));
	if (!new_env)
	{
		free(new_var);
		ft_putendl_fd("minishell: export: allocation failed", 2);
		return ;
	}
	while (k < j)
	{
		new_env[k] = shell->env[k];
		k++;
	}
	new_env[j] = new_var;
	new_env[j + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
}

void	add_or_update_env(char *name, char *value, t_shell *shell)
{
	char	*new_var;
	char	*tmp;

	if (value)
	{
		tmp = ft_strjoin(name, "=");
		if (!tmp)
		{
			ft_putendl_fd("minishell: export: allocation failed", 2);
			return ;
		}
		new_var = ft_strjoin(tmp, value);
		free(tmp);
	}
	else
		new_var = ft_strdup(name);
	if (!new_var)
	{
		ft_putendl_fd("minishell: export: allocation failed", 2);
		return ;
	}
	if (!update_env_var(name, new_var, shell))
		add_env_var(new_var, shell);
}

bool	is_valid_identifier(const char *s)
{
	int	i;

	i = 0;
	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
