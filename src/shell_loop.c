/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:36:52 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 12:38:58 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// No se necesita
// static void	process_input_line(char *line, t_shell *shell)
// {
// 	if (line[0] != '\0')
// 	{
// 		add_history(line);
		
// 		// Si la línea contiene operadores lógicos, usamos el nuevo procesador
// 		if (contains_logical_operators(line))
// 			process_logical_line(line, shell);
// 		else
// 			exec_line(line, shell);
// 	}
// }

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
			continue ;
		set_signals(MODE_SHELL);
		line = readline(shell->prompt);
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
