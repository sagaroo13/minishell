/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:59:23 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/08 14:59:23 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void exec_last(t_command *cmd, t_shell *shell)
{
    pid_t pid;
    int status;

    if (cmd->builtin)
    {
        int ret = exec_builtin(cmd->args, shell);
        update_last_exit_status(shell, ret);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0) // HIJO
    {
    // Señales por defecto para que Ctrl+C mate solo al hijo
    set_signals(MODE_CHILD);

        // Aplicar redirecciones si existen
        redirs(cmd);

        // Ejecutar comando externo
        exec(cmd->args[0], cmd->args, shell);

        // Si exec falla
        perror(cmd->args[0]);
        exit(127);
    }
    else // PADRE
    {
        // Ignorar Ctrl+C y Ctrl+\ mientras espera al hijo
        set_signals(MODE_PIPE);

        waitpid(pid, &status, 0);

        // Normalizar status si el hijo murió por señal
        if (WIFSIGNALED(status))
            update_last_exit_status(shell, 128 + WTERMSIG(status));
        else
            update_last_exit_status(shell, WEXITSTATUS(status));

        // Restaurar manejadores de la shell (Ctrl+C no mata la shell)
        set_signals(MODE_SHELL);
    }
}

// ------------------------- exec_pipe -------------------------
void exec_pipe(t_command *cmd, t_shell *shell)
{
    int pipe_fd[2];
    pid_t pid;
    int status;

    if (pipe(pipe_fd) == -1)
        exit(EXIT_FAILURE);

    pid = fork();
    if (pid == -1)
        exit(EXIT_FAILURE);

    if (pid == 0) // HIJO
    {
        set_signals(MODE_CHILD);

        // Configurar pipe para escritura
        safe_close(pipe_fd[0]);
        safe_dup2(pipe_fd[1], STDOUT_FILENO);
        safe_close(pipe_fd[1]);

        redirs(cmd);
        exec(cmd->args[0], cmd->args, shell);

        exit(127); // si exec falla
    }
    else // PADRE
    {
        set_signals(MODE_PIPE);

        // Configurar pipe para lectura
        safe_close(pipe_fd[1]);
        safe_dup2(pipe_fd[0], STDIN_FILENO);
        safe_close(pipe_fd[0]);

        waitpid(pid, &status, 0);

    // Normalizar exit status
        if (WIFSIGNALED(status))
            update_last_exit_status(shell, 128 + WTERMSIG(status));
        else
            update_last_exit_status(shell, WEXITSTATUS(status));

    // Volver a modo shell (Ctrl+C imprime nueva línea y prompt)
    set_signals(MODE_SHELL);
    }
}