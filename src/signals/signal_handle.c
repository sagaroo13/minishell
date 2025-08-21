/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:53:28 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 13:29:10 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	set_signals(int mode)
{
	if (mode == MODE_CHILD)
	{
		signal(SIGINT, SIG_DFL);
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
		signal(SIGINT, SIG_IGN);  // Parent ignora SIGINT durante heredoc
		signal(SIGQUIT, SIG_IGN);
	}
}
