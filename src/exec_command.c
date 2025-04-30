#include "../include/minishell.h"

char *find_path(char **envp)
{
	char *env_path;
	int i;

	env_path = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			env_path = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (env_path);
}

char *try_executable_path(char **paths, char *line)
{
	char *path;
	int i;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path, line);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (paths[0]);
}

char *get_path(char **envp, char *line)
{
	char *env_path;
	char **paths;
	char *executable_path;

	env_path = find_path(envp);
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	executable_path = try_executable_path(paths, line);
	free(paths);
	return (executable_path);
}

void free_args(char **args)
{
	int i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

// void print_redir(char **redir)
// {
// 	int i = 0;
// 	while (redir[i])
// 	{
// 		printf("Redirection %d: %s\n", i, redir[i]);
// 		i++;
// 	}
// }

void exec(char *cmd_name, char **cmd_lst, char **envp)
{
	char *path;

	path = get_path(envp, cmd_name);
	if (is_builtin(cmd_name))
		exec_builtin(cmd_lst, envp);
	else
		execve(path, cmd_lst, envp);
    perror("Error al ejecutar execvp");
    exit(EXIT_FAILURE);
}

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

// void print_all(char **args)
// {
// 	int i = 0;
// 	while (args[i])
// 	{
// 		printf("Arg %d: %s\n", i, args[i]);
// 		i++;
// 	}
// }

void exec_line(char *line, char **envp)
{
	char *cmds[MAX_CMDS];
	char *args[MAX_ARGS];
	char *redirs[MAX_REDIRS];
	int num_comandos;
	int i;
	(void)envp;

	tokenize(line, "|", cmds, MAX_CMDS);
	// print_all(cmds);
	i = -1;
	while (cmds[++i])
	{
		num_comandos = tokenize(cmds[i], " \t\n", args, MAX_ARGS);
		// print_all(args);
		process_redirs(args, redirs);

		if (i == 0 && redirs[0])
            redir_in(redirs[0]);
        if (i != num_comandos - 1)
            exec_pipe(args[0], args, envp, redirs[2]);
        else
            redir_out(args[0], args, envp, redirs[1], redirs[2]);
	}
}
