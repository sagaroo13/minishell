/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:44:17 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 18:59:06 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

static int	split_name_value(char *arg,
		char **name, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		*name = ft_substr(arg, 0, equal_pos - arg);
		if (!*name)
			return (1);
		*value = ft_strdup(equal_pos + 1);
	}
	else
	{
		*name = ft_strdup(arg);
		if (!*name)
			return (1);
		*value = NULL;
	}
	return (0);
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*equal_pos;

	name = NULL;
	value = NULL;
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("bash: export: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (1);
	}
	equal_pos = ft_strchr(arg, '=');
	if (split_name_value(arg, &name, &value))
		return (1);
	if (equal_pos)
		add_or_update_env(name, value, shell);
	free(name);
	free(value);
	return (0);
}

int	env_export(char **argv, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv || !argv[0] || !shell)
		return (1);
	if (!argv[1])
		return (display_sorted_exports(shell));
	while (argv[i])
	{
		if (process_export_arg(argv[i], shell))
			status = 1;
		i++;
	}
	set_exit_status_direct(shell, status);
	return (status);
}
