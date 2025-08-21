/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:35:53 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 13:10:39 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static void	print_echo_args(char **args, int i, int newline)
{
	if (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		i++;
		while (args[i])
		{
			write(STDOUT_FILENO, " ", 1);
			write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
			i++;
		}
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}

int	exec_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	expand_env_vars(args, shell);
	expand_exit_status(args, shell);
	while (args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	print_echo_args(args, i, newline);
	return (0);
}
