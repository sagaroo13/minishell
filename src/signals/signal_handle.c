/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:53:28 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 19:50:44 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	g_signal_received;

void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_received = 130;
	
}

void	sigint_handler_child(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_signal_received = 130;
	close(STDIN_FILENO);
	exit(130);
}

void	set_signals(int mode)
{
	if (mode == MODE_CHILD)
	{
		signal(SIGINT, sigint_handler_child);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == MODE_SHELL)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == MODE_PIPE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == MODE_HEREDOC)
	{
		signal(SIGINT, sigint_handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
}
