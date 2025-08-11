/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:53:28 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/08 17:51:11 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Manejador SIGINT en modo shell (Ctrl+C) - SIN rl_redisplay()
void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Manejador SIGQUIT en modo shell (Ctrl+\)
void	sigquit_handler(int sig)
{
	(void)sig;
	write (STDOUT_FILENO, "Quit\n", 5);
}

// Manejador SIGINT en procesos hijos
void	sigint_handler_in_child(int sig)
{
	(void)sig;
	exit (128 + sig);
}

// Manejador SIGQUIT en procesos hijos
void	sigquit_handler_in_child(int sig)
{
	(void)sig;
	write (STDOUT_FILENO, "Quit\n", 5);
	exit (128 + sig);
}

// Asignar manejadores según el modo
void	set_signals(int mode)
{
	if (mode == MODE_PIPE)
	{
		signal(SIGINT, sigint_handler_in_child);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == MODE_CHILD)
	{
		signal(SIGINT, sigint_handler_in_child);
		signal(SIGQUIT, sigquit_handler_in_child);
	}
	else if (mode == MODE_SHELL)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}
