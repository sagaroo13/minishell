#include "../include/minishell.h"

// Esta función trata de abrir de ejecutar un comando en un proceso hijo enlazando la entrada y salida de este por un pipe
// Tiene en cuenta tanto la posibilidad de redirección de error como ejecución en modo background
void	exec_pipe(t_command *cmd, char **envp)
{
	int     pipe_fd[2];
	pid_t   pid;
    int     status;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	if ((pid = fork()) == -1)
		exit(EXIT_FAILURE);
	if (!pid)
	{
		safe_close(pipe_fd[0]);
		safe_dup2(pipe_fd[1], STDOUT_FILENO);
        safe_close(pipe_fd[1]);
        redirs(cmd);
		exec(cmd->args[0], cmd->args, envp);
	}
	else
	{
		safe_close(pipe_fd[1]);
		safe_dup2(pipe_fd[0], STDIN_FILENO);
        safe_close(pipe_fd[0]);
		waitpid(pid, &status, 0);
	}
}

// Exec_out ejecuta el último comando de la línea en un proceso hijo y redirige su salida según se le indique (fd_out o STDOUT)
// Igual que en la función anterior tiene en cuenta que la ejecución pueda ser en background o que se redirija el error a un archivo
void exec_last(t_command *cmd, char **envp)
{
    pid_t pid;
	int status;

    if ((pid = fork()) == -1)
        exit(EXIT_FAILURE);
    if (!pid)
    {
        redirs(cmd);
        exec(cmd->args[0], cmd->args, envp);
    }
    else
    	waitpid(pid, &status, 0);
}
