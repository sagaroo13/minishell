/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:00:38 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 19:26:27 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_shell(t_shell *shell, char **envp)
{
	shell->env = copy_envp(envp);
	if (!shell->env)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("copy environment");
		exit(EXIT_FAILURE);
	}
	shell->last_status.status = 0;
	shell->last_status.last_exit_code = 0;
	shell->last_status.exit_called = false;
	shell->cat_count = 0;
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		ft_free_matrix(shell->env);
}

void	save_fds(t_stdfd *std)
{
	std->saved_stdin = -1;
	std->saved_stdout = -1;
	std->saved_stderr = -1;
	std->saved_stdin = safe_dup(STDIN_FILENO);
	std->saved_stdout = safe_dup(STDOUT_FILENO);
	std->saved_stderr = safe_dup(STDERR_FILENO);
}

void	restore_fds(t_stdfd *std)
{
	if (std->saved_stdin >= 0)
	{
		safe_dup2(std->saved_stdin, STDIN_FILENO);
		safe_close(std->saved_stdin);
	}
	if (std->saved_stdout >= 0)
	{
		safe_dup2(std->saved_stdout, STDOUT_FILENO);
		safe_close(std->saved_stdout);
	}
	if (std->saved_stderr >= 0)
	{
		safe_dup2(std->saved_stderr, STDERR_FILENO);
		safe_close(std->saved_stderr);
	}
}
