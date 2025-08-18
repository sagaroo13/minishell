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

char	**copy_envp(char **envp)
{
	int		i;
	int		n;
	char	**new_env;

	i = 0;
	n = 0;
	if (!envp)
		return (NULL);
	while (envp[n])
		n++;
	new_env = malloc(sizeof(char *) * (n + 1));
	if (!new_env)
		return (NULL);
	while (i < n)
	{
		new_env[i] = strdup(envp[i]);
		i++;
	}
	new_env[n] = NULL;
	return (new_env);
}

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

void	minishell(t_shell *shell)
{
	disable_echoctl();
	using_history();
	set_signals(MODE_SHELL);
	while (true)
	{
		safe_getcwd(shell->cwd, sizeof(shell->cwd));
		shell->prompt = ft_strjoin(shell->cwd, "$> ");
		save_fds(&shell->stdfd);
		shell->line = readline(shell->prompt);
		free(shell->prompt);
		if (!shell->line)
			break ;
		else
		{
			set_signals(MODE_SHELL);
			add_history(shell->line);
			exec_line(shell->line, shell);
		}
		free(shell->line);
		restore_fds(&shell->stdfd);
	}
	restore_terminal();
	clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.line = NULL;
	shell.prompt = NULL;
	shell.last_status.status = 0;
	shell.last_status.last_exit_code = 0;
	shell.last_status.exit_called = false;
	shell.env = copy_envp(envp);
	if (!shell.env)
	{
		perror("Failed to copy environment");
		return (EXIT_FAILURE);
	}
	printf(BANNER);
	minishell(&shell);
	return (EXIT_SUCCESS);
}
