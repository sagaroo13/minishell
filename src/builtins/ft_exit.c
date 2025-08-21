/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:19:08 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 13:10:48 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	exec_exit(char **args)
{
	int	code;

	write (STDOUT_FILENO, "exit\n", 5);
	if (!args[1])
		exit (0);
	if (!is_numeric(args[1]))
	{
		ft_putendl_fd("minishell: exit: numeric argument required", 2);
		exit (255);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	code = atoi(args[1]);
	exit (code);
}
