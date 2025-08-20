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
	t_shell	shell;

	ft_memset(&shell, 0, sizeof(t_shell));
	(void)argc;
	(void)argv;
	shell.env = copy_envp(envp);
	if (!shell.env)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("copy environment");
		return (EXIT_FAILURE);
	}
	shell.last_status.status = 0;
	shell.last_status.last_exit_code = 0;
	shell.last_status.exit_called = false;
	printf(BANNER);
	minishell(&shell);
	cleanup_shell(&shell);
	return (EXIT_SUCCESS);
}
