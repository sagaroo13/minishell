/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:38:38 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/07 14:38:38 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell			shell;
	int				exit_code;
	struct termios	term;

	(void)argc;
	(void)argv;
	setup_shell(&shell, envp);
	// Solo mostrar banner si la entrada es un terminal (no pipes/redirecciones)
	if (tcgetattr(STDIN_FILENO, &term) == 0)
		printf(BANNER);	
	minishell(&shell);
	exit_code = shell.last_status.last_exit_code;
	cleanup_shell(&shell);
	return (exit_code);
}
