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


void free_env(char **env)
{
    int i = 0;
    if (!env) return;
    while (env[i])
        free(env[i++]);
    free(env);
}

void cleanup_shell(t_shell *shell)
{
    if (shell->env)
        free_env(shell->env);
    // liberar otros mallocs de shell si los añades
}

char **copy_envp(char **envp)
{
    int n = 0;
    while (envp && envp[n]) n++;

    char **new_env = malloc(sizeof(char *) * (n + 1));
    if (!new_env) return NULL;

    for (int i = 0; i < n; i++)
        new_env[i] = strdup(envp[i]);

    new_env[n] = NULL;
    return new_env;
}

void save_fds(t_stdfd *std)
{
    std->saved_stdin  = safe_dup(STDIN_FILENO);
    std->saved_stdout = safe_dup(STDOUT_FILENO);
    std->saved_stderr = safe_dup(STDERR_FILENO);
}

void restore_fds(t_stdfd *std)
{
    safe_dup2(std->saved_stdin, STDIN_FILENO);
    safe_dup2(std->saved_stdout, STDOUT_FILENO);
    safe_dup2(std->saved_stderr, STDERR_FILENO);
    close(std->saved_stdin);
    close(std->saved_stdout);
    close(std->saved_stderr);
}

// --- lectura de input + historial
void minishell(t_shell *shell)
{
    char *line;

    disable_echoctl();
    using_history();

    // Configurar señales de shell antes del bucle
    set_signals(MODE_SHELL); // Ctrl+C solo imprime prompt
    rl_catch_signals = 0;    // readline no captura SIGINT

    while (true)
    {
        // Construir prompt con cwd
        safe_getcwd(shell->cwd, sizeof(shell->cwd));
        shell->prompt = ft_strjoin(shell->cwd, "$> ");
        if (!shell->prompt)
            continue; // si falla malloc, intentar de nuevo

        // Guardar fds originales
        save_fds(&shell->stdfd);

    // Asegurar que las señales están en modo shell antes de leer
    set_signals(MODE_SHELL);

        // Leer línea del usuario
        line = readline(shell->prompt);
        free(shell->prompt);

        if (!line) // Ctrl+D o EOF
            break;

        if (line[0] != '\0')
        {
            add_history(line);

            // Ejecutar la línea usando toda la info de shell
            exec_line(line, shell);
        }

        free(line);

        // Restaurar fds originales
        restore_fds(&shell->stdfd);
    }

    restore_terminal();
    clear_history();
}



int main(int argc, char **argv, char **envp)
{
    t_shell shell = {0}; // inicializa todos los campos a 0/NULL/false

    (void)argc;
    (void)argv;

    shell.env = copy_envp(envp);
    if (!shell.env)
    {
        perror("Failed to copy environment");
        return EXIT_FAILURE;
    }

    shell.last_status.status = 0;
    shell.last_status.last_exit_code = 0;
    shell.last_status.exit_called = false;

    printf(BANNER);

    minishell(&shell);

   cleanup_shell(&shell);
    return EXIT_SUCCESS;
}
