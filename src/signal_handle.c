#include "../include/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// -------------------- HANDLERS -------------------------
void    sigint_handler_in_process(int sig)
{
    (void) sig;
    write(STDOUT_FILENO, "\n", 1);
    signal(SIGINT, SIG_DFL); // Restaurar comportamiento por defecto
    raise(SIGINT); // Terminar el proceso correctamente
}

// Manejador para procesos hijos (Ctrl+\ debe imprimir mensaje y terminar)
void    sigquit_handler_in_process(int sig)
{
    (void) sig;
    write(STDOUT_FILENO, "Quit\n", 5);
    signal(SIGQUIT, SIG_DFL); // Restaurar comportamiento por defecto
    raise(SIGQUIT); // Terminar el proceso
}

// Manejador para Ctrl+C en heredoc (solo interrumpe la entrada)
void    sigint_handler_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
}

// Manejador para el shell interactivo (Ctrl+C limpia línea sin cerrar)
void    sigint_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

// Configurar señales según el estado de Minishell
void    set_signals(int mode)
{
    if (mode == MODE_SHELL)  // Modo interactivo
    {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
    }
    else if (mode == MODE_CHILD)  // Procesos hijos
    {
        signal(SIGINT, sigint_handler_in_process);
        signal(SIGQUIT, sigquit_handler_in_process);
    }
    else if (mode == MODE_HEREDOC)  // Heredoc
    {
        signal(SIGINT, sigint_handler_heredoc);
        signal(SIGQUIT, SIG_IGN);
    }
}
