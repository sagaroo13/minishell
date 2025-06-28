#include "../include/minishell.h"

void save_fds(t_stdfd *std)
{
    std->saved_stdin  = safe_dup(STDIN_FILENO);
    std->saved_stdout = safe_dup(STDOUT_FILENO);
    std->saved_stderr = safe_dup(STDERR_FILENO);
}

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
    char *line;
    char *prompt;
    char cwd[BUFFER_SIZE];

    disable_echoctl();  // Aplicar configuración antes de iniciar el shell
    set_signals(MODE_SHELL);
    using_history();

    (void)envp;  // Evitar warning por envp no usado
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
