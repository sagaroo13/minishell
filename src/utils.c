/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:00:38 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 14:54:35 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Cambios
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
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		ft_free_matrix(shell->env);
}

char	*get_env(t_shell *shell, const char *name)
{
	int		i;
	char	*eq;

	i = 0;
	if (!shell || !name)
		return (NULL);
	while (shell->env && shell->env[i])
	{
		eq = ft_strchr(shell->env[i], '=');
		if (eq && ft_strncmp(shell->env[i], name, eq - shell->env[i]) == 0
			&& (size_t)(eq - shell->env[i]) == ft_strlen(name))
			return (eq + 1);
		i++;
	}
	return (NULL);
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