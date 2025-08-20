/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:49:56 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 00:49:57 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
