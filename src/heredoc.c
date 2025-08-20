/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:30:14 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/20 15:02:14 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

/*
 * Handler SIGINT para heredoc: solo operaciones async-signal-safe.
 * No llamar printf/close/get_next_line/exit aquí.
 */
static void	_sigint_heredoc(int sig)
{
    (void)sig;
    _exit(130); /* 128 + SIGINT */
}

/*
 * Lee líneas desde el TTY real (si existe) para evitar cuelgues cuando STDIN
 * está redirigido (ej: cat << EOF). Escribe el contenido en pipe_fd[1].
 * Termina con éxito al ver el delimitador o EOF.
 */
static void	read_from_stdin(int pipe_fd[2], char *delim)
{
    int	  tty_fd;
    int	  in_fd;
    char *line;
    char *nl;

    close(pipe_fd[0]); /* solo escritura en el hijo */

    tty_fd = open("/dev/tty", O_RDONLY);
    in_fd = (tty_fd >= 0) ? tty_fd : STDIN_FILENO;

    while (1)
    {
        write(STDOUT_FILENO, "heredoc> ", 9);
        line = get_next_line(in_fd);
        if (!line)
            break; /* EOF o señal */
        nl = ft_strchr(line, '\n');
        if (nl)
            *nl = '\0';
        if (!ft_strcmp(delim, line))
        {
            free(line);
            break; /* delimitador encontrado */
        }
        ft_putendl_fd(line, pipe_fd[1]);
        free(line);
    }

    if (tty_fd >= 0)
        close(tty_fd);
    close(pipe_fd[1]);              /* despertar al padre */
    get_next_line(-1);              /* limpiar buffers internos de GNL */
    _exit(EXIT_SUCCESS);
}


void		  heredoc(t_command *cmd)
{
    int	  i;
    int	  pipe_fd[2];
    int	  last_fd;
    pid_t pid;
    int	  status;

    if (cmd->heredoc.n_redirs < 1)
        return;

    last_fd = -1;
    i = 0;
    while (i < cmd->heredoc.n_redirs)
    {
        if (pipe(pipe_fd) == -1)
            exit(EXIT_FAILURE);

        pid = fork();
        if (pid == -1)
            exit(EXIT_FAILURE);

        if (pid == 0)
        {
            signal(SIGINT, _sigint_heredoc);
             signal(SIGQUIT, SIG_IGN);
            read_from_stdin(pipe_fd, cmd->heredoc.redirs[i]); /* no vuelve */
        }
        /* padre */
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        close(pipe_fd[1]);
        while (waitpid(pid, &status, 0) == -1 && errno == EINTR) {}

        /* si fue interrumpido por SIGINT (señal) o salió con 130, abortar */
        if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
            || (WIFEXITED(status) && WEXITSTATUS(status) == 130))
        {
            write(STDOUT_FILENO, "\n", 1);
            close(pipe_fd[0]);
            if (last_fd != -1)
                close(last_fd);
            // Cancelar la ejecución de esta línea de comandos
            if (cmd && cmd->cmd_line)
                cmd->cmd_line->execute = false;
            if (cmd && cmd->shell)
                update_last_exit_status(cmd->shell, 130);
            set_signals(MODE_SHELL);
            return;
        }

        /* si es el último heredoc, conservar FD de lectura para STDIN */
        if (i == cmd->heredoc.n_redirs - 1)
            last_fd = pipe_fd[0];
        else
            close(pipe_fd[0]);

        i++;
    }

    if (last_fd != -1)
    {
        dup2(last_fd, STDIN_FILENO);
        close(last_fd);
    }
    set_signals(MODE_SHELL);
}
