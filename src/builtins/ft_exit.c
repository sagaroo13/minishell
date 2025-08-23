/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:19:08 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 10:46:35 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_exit_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '+' || str[0] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static long	safe_atol(char *str, int *overflow)
{
	long	r;
	int		i;
	int		sign;

	r = 0;
	i = 0;
	sign = 1;
	*overflow = 0;
	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[0] == '+')
		i++;
	while (str[i])
	{
		if (r > (LONG_MAX - (str[i] - '0')) / 10)
			return (*overflow = 1, 0);
		r = r * 10 + (str[i++] - '0');
	}
	return (r * sign);
}

static int	process_exit_args(char **args, t_shell *shell)
{
	long	code;
	int		ovf;

	if (!args[1])
		return (shell->last_status.last_exit_code);
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (-1);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (-2);
	}
	code = safe_atol(args[1], &ovf);
	if (ovf)
	{
		print_exit_numeric_error(args[1]);
		return (-2);
	}
	return ((int)((code % 256 + 256) % 256));
}

int	exec_exit(char **args, t_shell *shell)
{
	int	code;

	printf("exit\n");
	code = process_exit_args(args, shell);
	if (code == -1)
		return (1);
	if (code == -2)
	{
		restore_terminal();
		rl_clear_history();
		exit(2);
	}
	restore_terminal();
	rl_clear_history();
	exit(code);
}
