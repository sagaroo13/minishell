/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:59:23 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 21:00:00 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static bool	is_interactive_command(const char *cmd_name)
{
	if (!cmd_name)
		return (false);
	return (ft_strcmp((char*)cmd_name, "cat") == 0 || 
			ft_strcmp((char*)cmd_name, "/bin/cat") == 0);
}

void	exec_last(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		ret;

	if (cmd->builtin)
	{
		// Verificar redirecciones antes de ejecutar el built-in
		if (redirs(cmd) != 0)
		{
			set_exit_status_direct(shell, 1);
			return ;
		}
		ret = exec_builtin(cmd->args, shell);
		set_exit_status_direct(shell, ret);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("fork");
		return ;
	}
	if (pid == 0)
		child_exec_command(cmd, shell);
	parent_wait_and_finalize(shell, pid);
}
/**
 * @brief Configura los descriptores de archivo y redirecciones para un comando interactivo
 * 
 * @param cmd Estructura del comando
 * @param shell Estructura principal del shell
 * @param pipe_fd Array con los descriptores del pipe
 * @return int 1 si el comando tiene redirecciones y debe ejecutarse normalmente, 0 si no
 */
static int	setup_interactive_pipe(t_command *cmd, t_shell *shell, int pipe_fd[2])
{
	set_signals(MODE_CHILD);
	safe_close(pipe_fd[0]);
	safe_dup2(pipe_fd[1], STDOUT_FILENO);
	safe_close(pipe_fd[1]);
	if (redirs(cmd) != 0)
		exit(1); // Error en redirecciones
	
	// Si hay redirecciones, ejecutar el comando normalmente
	if (cmd->stdin.n_redirs > 0 || cmd->stdout.n_redirs > 0 || 
		cmd->append.n_redirs > 0 || cmd->stderr.n_redirs > 0 || 
		cmd->heredoc.n_redirs > 0)
	{
		exec(cmd->args[0], cmd->args, shell);
		if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
			exit(126);
		else
			exit(127);
		return (1);
	}
	return (0);
}

/**
 * @brief Procesa la entrada de un comando interactivo como 'cat'
 * 
 * @param void No recibe parámetros
 */
static void	process_interactive_input(void)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	int		last_was_newline = 0;
	
	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
		if (bytes_read <= 0) 
			exit(0); 
		if (bytes_read == 1 && buffer[0] == '\n')
		{
			if (last_was_newline) 
				exit(0);
			last_was_newline = 1;
		} 
		else 
		{
			last_was_newline = 0;
			write(STDOUT_FILENO, buffer, bytes_read);
		}
	}
}

/**
 * @brief Ejecuta un comando interactivo en un pipe
 * 
 * @param cmd Estructura del comando
 * @param shell Estructura principal del shell
 * @param pipe_fd Array con los descriptores del pipe
 */
void	child_exec_interactive_pipe(t_command *cmd, t_shell *shell, int pipe_fd[2])
{
	// Configurar los descriptores de archivo y redirecciones
	if (setup_interactive_pipe(cmd, shell, pipe_fd))
		return;
	
	// Procesar la entrada interactivamente
	process_interactive_input();
}
void	exec_pipe(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		if (is_interactive_command(cmd->args[0]))
			child_exec_interactive_pipe(cmd, shell, pipe_fd);
		else
			child_exec_pipe(cmd, shell, pipe_fd);
	}
	else
		parent_setup_pipe_and_wait(shell, pipe_fd, pid);
}
