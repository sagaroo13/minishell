/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_off.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:44:41 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/23 15:49:00 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

struct termios original_termios; // Estructura global para almacenar la configuración original de la terminalvb 

// Restaurar la configuración de la terminal al salir
void restore_terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}
 
// Desactivar impresión de `^C`
void	disable_echoctl(void)
{
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, &original_termios);
	new_termios = original_termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}
