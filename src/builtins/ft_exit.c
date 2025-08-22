/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:19:08 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 16:33:00 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <limits.h>

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	
	// Permitir signo al principio
	if (str[0] == '+' || str[0] == '-')
		i++;
	
	// Debe haber al menos un dígito después del signo (si hay)
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
	long	result;
	long	sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
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
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			*overflow = 1;
			return (0);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	process_exit_args(char **args, t_shell *shell)
{
	long	exit_code;
	int		overflow;
	
	// Sin argumentos: usar el último código de salida
	if (!args[1])
		return (shell->last_status.last_exit_code);
	
	// Demasiados argumentos: error pero NO salir
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (-1); // Código especial para indicar error sin salir
	}
	
	// Argumento no numérico
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (-2); // Código especial para argumentos no numéricos
	}
	
	// Convertir a número y manejar overflow
	exit_code = safe_atol(args[1], &overflow);
	if (overflow)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (-2);
	}
	
	// Aplicar módulo 256 para obtener código de salida válido
	return ((int)(exit_code % 256 + 256) % 256);
}

int	exec_exit(char **args, t_shell *shell)
{
	int	code;

	printf("exit\n");
	code = process_exit_args(args, shell);
	
	// Si hay demasiados argumentos, NO salir, solo retornar error
	if (code == -1)
		return (1);
	
	// Para argumentos no numéricos o overflow, salir con código 2
	if (code == -2)
	{
		restore_terminal();
		rl_clear_history();
		exit(2);
	}
	
	// Salir normalmente con el código calculado
	restore_terminal();
	rl_clear_history();
	exit(code);
}
