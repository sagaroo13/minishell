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

// main.c
t_last_status	g_last_exit_status = {0, 0, false};

void	save_fds(t_stdfd *std)
{
	std->saved_stdin = safe_dup(STDIN_FILENO);
	std->saved_stdout = safe_dup(STDOUT_FILENO);
	std->saved_stderr = safe_dup(STDERR_FILENO);
}

void	restore_fds(t_stdfd *std)
{
	safe_dup2(std->saved_stdin, STDIN_FILENO);
	safe_dup2(std->saved_stdout, STDOUT_FILENO);
	safe_dup2(std->saved_stderr, STDERR_FILENO);
	close(std->saved_stdin);
	close(std->saved_stdout);
	close(std->saved_stderr);
}

void	minishell(char **envp)
{
	t_shell_data	shell;

	disable_echoctl();
	using_history();
	set_signals(MODE_SHELL);
	while (true)
	{
		safe_getcwd(shell.cwd, sizeof(shell.cwd));
		shell.prompt = ft_strjoin(shell.cwd, "$> ");
		save_fds(&shell.stdfd);
		shell.line = readline(shell.prompt);
		free(shell.prompt);
		if (!shell.line)
			break ;
		else
		{
			set_signals(MODE_SHELL);
			add_history(shell.line);
			exec_line(shell.line, envp);
		}
		free(shell.line);
		restore_fds(&shell.stdfd);
	}
	restore_terminal();
	clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	printf(BANNER);
	minishell(envp);
	return (EXIT_SUCCESS);
}
