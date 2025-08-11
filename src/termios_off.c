/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_off.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:44:41 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/08 17:55:23 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

struct termios	g_original_termios; // Estructura global 

// Restaurar la configuración de la terminal al salir
void	restore_terminal(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &g_original_termios);
}

// Desactivar impresión de `^C`
void	disable_echoctl(void)
{
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, &g_original_termios);
	new_termios = g_original_termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}
