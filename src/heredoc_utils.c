/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:06:42 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 01:11:09 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	_sigint_heredoc(int sig)
{
	(void)sig;
	exit(130);
}

pid_t	start_heredoc_child(int pipe_fd[2], const char *delim)
{
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, _sigint_heredoc);
		signal(SIGQUIT, SIG_IGN);
		read_from_stdin(pipe_fd, (char *)delim);
	}
	return (pid);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			break ;
	}
	return (status);
}

static bool	handle_child_status(t_command *cmd, int status, t_heredoc_ctx *ctx)
{
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		write(STDOUT_FILENO, "\n", 1);
		if (*(ctx->plast_fd) != -1)
			close(*(ctx->plast_fd));
		close(ctx->pipe_fd[0]);
		if (cmd && cmd->cmd_line)
			cmd->cmd_line->execute = false;
		if (cmd && cmd->shell)
			update_last_exit_status(cmd->shell, 130);
		set_signals(MODE_SHELL);
		return (false);
	}
	if (ctx->is_last)
		*(ctx->plast_fd) = ctx->pipe_fd[0];
	else
		close(ctx->pipe_fd[0]);
	return (true);
}

bool	handle_parent_after_child(t_command *cmd, pid_t pid, t_heredoc_ctx *ctx)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close(ctx->pipe_fd[1]);
	status = wait_for_child(pid);
	return (handle_child_status(cmd, status, ctx));
}
