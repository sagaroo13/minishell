#include "../include/minishell.h"

void save_fds(t_stdfd *std)
{
    std->saved_stdin  = safe_dup(STDIN_FILENO);
    std->saved_stdout = safe_dup(STDOUT_FILENO);
    std->saved_stderr = safe_dup(STDERR_FILENO);
}

/* Restaura los fds originales desde el backup y cierra los backups. */
void restore_fds(t_stdfd *std)
{
    safe_dup2(std->saved_stdin,  STDIN_FILENO);
    safe_dup2(std->saved_stdout, STDOUT_FILENO);
    safe_dup2(std->saved_stderr, STDERR_FILENO);

    close(std->saved_stdin);
    close(std->saved_stdout);
    close(std->saved_stderr);
}

void minishell(char **envp)
{
    char	*line;
    char	cwd[BUFFER_SIZE];
    t_stdfd	std;

    using_history();

    while (true)
    {
        /* 1) Mostrar prompt */
        safe_getcwd(cwd, sizeof(cwd));
        printf(CYAN BOLD "%s$> " RESET, cwd);

        /* 2) Guardar descriptores originales */
        save_fds(&std);

        /* 3) Leer línea */
        line = readline("");
        if (!line)   // EOF (Ctrl+D)
        {
            write(1, "\n", 1);
            break;
        }

        if (*line)   // línea no vacía
        {
            add_history(line);
            exec_line(line, envp);
        }
        free(line);

        /* 4) Restaurar descriptores para el prompt siguiente */
        restore_fds(&std);
    }

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
