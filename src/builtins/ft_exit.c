/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:19:08 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 02:44:30 by shirakim         ###   ########.fr       */
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

static int	process_exit_args(char **args, t_shell *shell)
{
	(void)shell; // Indicar que el parámetro shell no se utiliza
	
	if (!args[1])
		return (-1);
	if (!is_numeric(args[1]))
		return (-2);
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	return (ft_atoi(args[1]));
}

int	exec_exit(char **args, t_shell *shell)
{
	int	code;

	write(STDOUT_FILENO, "exit\n", 5);
	code = process_exit_args(args, shell);
	if (code == 1)
		return (1);
	restore_terminal();
	rl_clear_history();
	if (code == -1)
		code = shell->last_status.last_exit_code;
	else if (code == -2)
		code = 255;
	exit(code);
}
