/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_display.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:27:16 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 18:57:49 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**create_env_copy(char **envp, int len)
{
	char	**sorted_env;
	int		i;

	sorted_env = (char **)malloc(sizeof(char *) * (len + 1));
	if (!sorted_env)
		return (NULL);
	i = -1;
	while (++i < len)
		sorted_env[i] = ft_strdup(envp[i]);
	sorted_env[len] = NULL;
	return (sorted_env);
}

static void	bubble_sort_env(char **sorted_env, int len)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (++i < len - 1)
	{
		j = -1;
		while (++j < len - i - 1)
		{
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
		}
	}
}

static char	**sort_env(char **envp)
{
	int		len;
	char	**sorted_env;

	len = 0;
	while (envp[len])
		len++;
	sorted_env = create_env_copy(envp, len);
	if (!sorted_env)
		return (NULL);
	bubble_sort_env(sorted_env, len);
	return (sorted_env);
}

static void	print_export_var(char *var)
{
	char	*eq_pos;
	int		name_len;

	eq_pos = ft_strchr(var, '=');
	if (eq_pos)
	{
		name_len = eq_pos - var;
		write(1, "declare -x ", 11);
		write(1, var, name_len + 1);
		write(1, "\"", 1);
		write(1, eq_pos + 1, ft_strlen(eq_pos + 1));
		write(1, "\"", 1);
		write(1, "\n", 1);
	}
	else
	{
		write(1, "declare -x ", 11);
		write(1, var, ft_strlen(var));
		write(1, "\n", 1);
	}
}

int	display_sorted_exports(t_shell *shell)
{
	char	**sorted_env;
	int		i;

	sorted_env = sort_env(shell->env);
	if (!sorted_env)
		return (1);
	i = 0;
	while (sorted_env[i])
	{
		print_export_var(sorted_env[i]);
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
	return (0);
}
