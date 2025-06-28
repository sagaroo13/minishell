#include "../include/minishell.h"

// Esta función simplemente abre un archivo y redirige la entrada estándar a su descriptor de fichero
void    redir_in(char *file_in)
{
    int fd_in;

    fd_in = safe_open(file_in, READ);
    safe_dup2(fd_in, STDIN_FILENO);
    safe_close(fd_in);
}


// Esta función trata de abrir de ejecutar un comando en un proceso hijo enlazando la entrada y salida de este por un pipe
// Tiene en cuenta tanto la posibilidad de redirección de error como ejecución en modo background
void	exec_pipe(char *cmd_name, char **cmd_lst, char **envp, char *stderr_file)
{
	int	pipe_fd[2];
	pid_t pid;
	int fd_err;
	int status;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	if ((pid = fork()) == -1)
		exit(EXIT_FAILURE);
	if (!pid)
	{
        set_signals(MODE_CHILD);
        printf("Este es el proceso hijo: %d\n", getpid());
		safe_close(pipe_fd[0]);
		safe_dup2(pipe_fd[1], STDOUT_FILENO);
        safe_close(pipe_fd[1]);
        if (stderr_file)
        {
            fd_err = safe_open(stderr_file, WRITE);
            safe_dup2(fd_err, STDERR_FILENO);
            safe_close(fd_err);
        }
		exec(cmd_name, cmd_lst, envp);
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
void redir_out(char *cmd_name, char **cmd_lst, char **envp, char *stdout_file, char *stderr_file)
{
    pid_t pid;
	int fd_out;
	int fd_err;
	int status;

    if ((pid = fork()) == -1)
        exit(EXIT_FAILURE);
    if (!pid)
    {
        if (stdout_file)
        {
            set_signals(MODE_CHILD);
            printf("Este es el proceso hijo: %d\n", getpid());
            fd_out = safe_open(stdout_file, WRITE);
            safe_dup2(fd_out, STDOUT_FILENO);
			safe_close(fd_out);
        }
        if (stderr_file)
        {
            fd_err = safe_open(stderr_file, WRITE);
			safe_dup2(fd_err, STDERR_FILENO);
			safe_close(fd_err);
        }
        exec(cmd_name, cmd_lst, envp);
    }
    else
    	waitpid(pid, &status, 0);
}
