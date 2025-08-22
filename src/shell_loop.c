/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:36:52 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 18:54:02 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool	process_command_line(char *line, t_shell *shell)
{
	if (!line)
		return (false);
	if (*line && ft_strlen(line) > 0)
	{
		add_history(line);
		exec_line(line, shell);
	}
	free(line);
	return (true);
}

static char	*get_command_line(t_shell *shell)
{
	char			*line;
	char			*prompt_to_use;
	struct termios	term;

	safe_getcwd(shell->cwd, sizeof(shell->cwd));
	shell->prompt = ft_strjoin(shell->cwd, "$> ");
	if (!shell->prompt)
		return (NULL);
	set_signals(MODE_SHELL);
	// Si la entrada no es un terminal (es un pipe), no mostrar prompt
	if (tcgetattr(STDIN_FILENO, &term) == 0)
		prompt_to_use = shell->prompt;
	else
		prompt_to_use = NULL;		
	line = readline(prompt_to_use);
	shell->line = line;
	free(shell->prompt);
	return (line);
}

void	minishell(t_shell *shell)
{
	char	*line;

	g_signal_received = 0;
	disable_echoctl();
	set_signals(MODE_SHELL);
	rl_catch_signals = 0;
	while (true)
	{
		save_fds(&shell->stdfd);
		line = get_command_line(shell);
		
		/* Verificar signal después de readline */
		if (g_signal_received)
		{
			shell->last_status.last_exit_code = g_signal_received;
			g_signal_received = 0;
		}
		
		if (!line || !process_command_line(line, shell))
			break;
		restore_fds(&shell->stdfd);
	}
	restore_terminal();
	rl_clear_history();
}
