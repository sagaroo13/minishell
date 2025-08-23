/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_config.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:45:42 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 13:13:23 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** is_interactive_terminal - Verifica si estamos en una terminal interactiva
** @return: 1 si estamos en terminal interactiva, 0 en caso contrario
**
** Esta función se usa para determinar si debemos mostrar el prompt y
** configurar el modo de terminal. En pipes o redirecciones retorna 0.
*/
int	is_interactive_terminal(void)
{
	struct termios	term;

	return (tcgetattr(STDIN_FILENO, &term) == 0);
}

/*
** configure_input_mode - Configura el modo de entrada de la terminal
** @param echo_ctl: 1 para activar ECHOCTL, 0 para desactivarlo
** @return: 0 en éxito, -1 en error
**
** ECHOCTL controla si los caracteres de control (Ctrl+C, Ctrl+\) se
** muestran en pantalla (^C, ^\). Lo desactivamos para tener una
** interfaz más limpia, similar a bash.
*/
int	configure_input_mode(int echo_ctl)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return (-1);
	if (echo_ctl)
		term.c_lflag |= ECHOCTL;
	else
		term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (-1);
	return (0);
}

/*
** disable_ctrl_chars - Desactiva la visualización de caracteres de control
*/
void	disable_ctrl_chars(void)
{
	configure_input_mode(0);
}

/*
** restore_ctrl_chars - Restaura la visualización normal de caracteres de control
*/
void	restore_ctrl_chars(void)
{
	configure_input_mode(1);
}
