/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:36:52 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 01:37:12 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	process_input_line(char *line, t_shell *shell)
{
	if (line[0] != '\0')
	{
		add_history(line);
		exec_line(line, shell);
	}
}

void	minishell(t_shell *shell)
{
	char	*line;

	disable_echoctl();
	set_signals(MODE_SHELL);
	rl_catch_signals = 0;
	while (true)
	{
		safe_getcwd(shell->cwd, sizeof(shell->cwd));
		shell->prompt = ft_strjoin(shell->cwd, "$> ");
		if (!shell->prompt)
			continue ;
		save_fds(&shell->stdfd);
		set_signals(MODE_SHELL);
		line = readline(shell->prompt);
		free(shell->prompt);
		if (!line)
			break ;
		process_input_line(line, shell);
		free(line);
		restore_fds(&shell->stdfd);
	}
	restore_terminal();
	rl_clear_history();
}
