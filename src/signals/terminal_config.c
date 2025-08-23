/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.config.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:30:33 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/23 19:30:47 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_interactive_terminal(void)
{
	struct termios	term;

	return (tcgetattr(STDIN_FILENO, &term) == 0);
}

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

void	disable_ctrl_chars(void)
{
	configure_input_mode(0);
}

void	restore_ctrl_chars(void)
{
	configure_input_mode(1);
}
