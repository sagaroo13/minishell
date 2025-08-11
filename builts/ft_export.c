/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:44:17 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/21 22:34:01 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	env_export(char **argv, char **envp)
{
	int		i;
	char	*new_var;

	i = 0;
	if (!argv || !argv[0] || !envp)
		return (0);
	if (!argv[1])
		return (exec_env(envp), 0);
	if (!is_valid_identifier(argv[1]))
		return (ft_putstr_fd("export: not a valid\n", 2), 1);
	if (ft_strchr(argv[1], '='))
		new_var = ft_strdup(argv[1]);
	else
		new_var = ft_strjoin(argv[1], "=");
	if (!new_var)
		return (1);
	while (envp[i] != NULL)
		i++;
	envp[i] = new_var;
	envp[i + 1] = NULL;
	return (0);
}
