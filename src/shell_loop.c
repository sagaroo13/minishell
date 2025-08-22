/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:36:52 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 18:05:21 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief Procesa una línea de comando ingresada por el usuario
 * 
 * @param line La línea de comando a procesar
 * @param shell Estructura principal del shell
 * @return true si se debe continuar el bucle, false si se debe salir
 */
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

/**
 * @brief Prepara el prompt y obtiene la línea de comando
 * 
 * @param shell Estructura principal del shell
 * @return La línea de comando ingresada o NULL en caso de error o EOF
 */
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

	g_shell = shell;
	disable_echoctl();
	set_signals(MODE_SHELL);
	rl_catch_signals = 0;
	while (true)
	{
		save_fds(&shell->stdfd);
		line = get_command_line(shell);
		if (!line || !process_command_line(line, shell))
			break;
		restore_fds(&shell->stdfd);
	}
	restore_terminal();
	rl_clear_history();
	g_shell = NULL;
}
