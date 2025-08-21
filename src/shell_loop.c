/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:36:52 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 16:32:21 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	minishell(t_shell *shell)
{
	char	*line;

	disable_echoctl();
	set_signals(MODE_SHELL);
	rl_catch_signals = 0;
	while (true)
	{
		save_fds(&shell->stdfd);
		safe_getcwd(shell->cwd, sizeof(shell->cwd));
		shell->prompt = ft_strjoin(shell->cwd, "$> ");
		if (!shell->prompt)
			break ;
		set_signals(MODE_SHELL);
		line = readline(shell->prompt);
		add_history(line);
		shell->line = line;
		free(shell->prompt);
		if (!line)
			break ;
		exec_line(line, shell);
		free(line);
		restore_fds(&shell->stdfd);
	}
	restore_terminal();
	rl_clear_history();
}
