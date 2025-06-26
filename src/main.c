#include "../include/minishell.h"
#include <termios.h>

void minishell(char **envp)
{
    char *line;
    char *prompt;
    char cwd[BUFFER_SIZE];

    disable_echoctl();  // Aplicar configuración antes de iniciar el shell
    set_signals(MODE_CHILD);
    using_history();

    while (true)
    {
        safe_getcwd(cwd, sizeof(cwd));

        prompt = ft_strjoin(cwd, "$> ");
        line = readline(prompt);
        free(prompt);

        if (!line)
        {
            write(1, "\n", 1);
            break;
        }
        if (*line)
        {
            add_history(line);
            exec_line(line, envp);
        }
        free(line);
    }

    restore_terminal();  // Restaurar configuración al salir
    clear_history();
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	
	banner();
	minishell(envp);
	return (EXIT_SUCCESS);
}
