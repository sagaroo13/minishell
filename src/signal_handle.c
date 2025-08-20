/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:53:28 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 14:32:43 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* Ctrl+C en heredoc */
void    sigint_handler_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    close(STDIN_FILENO);   // hace que readline/get_next_line terminen
}

void    set_signals(int mode)
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
    // heredoc tendrá su propio manejador en el proceso hijo;
    // aquí el padre ignora señales mientras espera
    signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
}