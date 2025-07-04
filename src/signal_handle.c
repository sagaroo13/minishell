#include "../include/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// -------------------- HANDLERS -------------------------
void sigint_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

// Manejador SIGQUIT en modo shell (Ctrl+\)
void sigquit_handler(int sig)
{
    (void)sig;
    // No hace nada, simplemente ignora Ctrl+
    rl_on_new_line();
    rl_redisplay();
}

// Manejador SIGINT en procesos hijos
void sigint_handler_in_child(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    exit(130);
}

// Manejador SIGQUIT en procesos hijos
void sigquit_handler_in_child(int sig)
{
    (void)sig;
    exit(131);
}

// Manejador SIGINT en heredoc (Ctrl+C interrumpe heredoc)
void sigint_handler_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    exit(130);  // Interrumpir heredoc
}

// Manejo especial de EOF (Ctrl+D)
void handle_ctrl_d(void)
{
    write(STDOUT_FILENO, "exit\n", 5);
    exit(0);
}

// Asignar manejadores de señales según el modo
void set_signals(int mode)
{
    if (mode == MODE_SHELL)
    {
        signal(SIGINT, sigint_handler);     // Ctrl+C limpia línea y muestra nuevo prompt
        signal(SIGQUIT, SIG_IGN);           // Ctrl+\ ignorado
    }
    else if (mode == MODE_CHILD)
    {
        usleep(100);  // Pequeña pausa para evitar problemas de sincronización
        signal(SIGINT, SIG_DFL);            // Restaurar comportamiento por defecto
        signal(SIGQUIT, SIG_DFL);           // Igual para Ctrl+
    }
    else if (mode == MODE_HEREDOC)
    {
        signal(SIGINT, sigint_handler_heredoc);
        signal(SIGQUIT, SIG_IGN);
    }
}

