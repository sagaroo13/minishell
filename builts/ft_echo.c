/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:35:53 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/09 13:29:38 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_n_flag(char *s)
{
	int	i;

	i = 1;
	if (!s || s[0] != '-')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	exec_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	expand_exit_status(args, &g_last_exit_status);
	if (args[i])
	{
		write (STDOUT_FILENO, args[i], ft_strlen(args[i]));
		i++;
		while (args[i])
		{
			write (STDOUT_FILENO, " ", 1);
			write (STDOUT_FILENO, args[i], ft_strlen(args[i]));
			i++;
		}
	}
	if (newline)
		write (STDOUT_FILENO, "\n", 1);
	return (0);
}
