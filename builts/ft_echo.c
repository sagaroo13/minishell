/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:35:53 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/14 14:18:09 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	is_n_flag(char *s)
{
	if (!s || s[0] != '-')
		return (0);
	for (int i = 1; s[i]; i++)
		if (s[i] != 'n')
			return (0);
	return (1);
}

int	exec_echo(char **args)
{
	int	i = 1;
	int	newline = 1;

	// Manejo de múltiples -n
	while (args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}

	// Imprimir los argumentos
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}

	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}


