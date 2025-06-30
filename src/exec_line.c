#include "../include/minishell.h"

// char *find_path(char **envp)
// {
// 	char *env_path;
// 	int i;

// 	env_path = NULL;
// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
// 		{
// 			env_path = envp[i] + 5;
// 			break ;
// 		}
// 		i++;
// 	}
// 	return (env_path);
// }

void redir_out_builtin(char *file, int stdfd)
{
	int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Redir (builtin)");
		return;
	}
	dup2(fd, stdfd);
	close(fd);
}
char *try_executable_path(char **paths, char *line)
{
	char *path_part;
	char *path;
	int i;

	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin("/", line);
		path = ft_strjoin(paths[i], path_part);
		free(path_part);
		if (!access(path, F_OK))
			return (path);
		free(path);
	}
	return (line);
}

char *get_path(char *line)
{
	char *env_path;
	char **paths;
	char *executable_path;

	env_path = getenv("PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	executable_path = try_executable_path(paths, line);
	ft_free_matrix(paths);
	return (executable_path);
}

void exec_line(char *line, char **envp)
{
    char *cmds[MAX_CMDS];
    char *args[MAX_ARGS];
    char *redirs[MAX_REDIRS];
    int num_cmds;
    int i;

    num_cmds = tokenize(line, "|", cmds, MAX_CMDS);
    print_all(cmds);

    // CASO: Solo un comando
    if (num_cmds == 1)
    {
        tokenize(cmds[0], " \t\n", args, MAX_ARGS);
        print_all(args);
        process_redirs(args, redirs);

        if (is_builtin(args[0]))
        {
            if (redirs[0]) redir_in(redirs[0]);           // < input
            if (redirs[1]) redir_out_builtin(redirs[1], STDOUT_FILENO); // > output
            if (redirs[2]) redir_out_builtin(redirs[2], STDERR_FILENO); // 2> error
            exec_builtin(args, envp);
        }
        else
        {
            redir_out(args[0], args, envp, redirs[1], redirs[2]);
        }
        return;
    }

    // CASO: Comandos con pipes
    for (i = 0; i < num_cmds; i++)
    {
        tokenize(cmds[i], " \t\n", args, MAX_ARGS);
        print_all(args);
        process_redirs(args, redirs);

        if (i == 0 && redirs[0])           // Redirección de entrada solo en el primero
            redir_in(redirs[0]);

        if (i != num_cmds - 1)             // Pipe entre comandos intermedios
            exec_pipe(args[0], args, envp, redirs[2]);
        else                               // Último comando
            redir_out(args[0], args, envp, redirs[1], redirs[2]);
    }
}



void exec(char *cmd_name, char **cmd_lst, char **envp)
{
	char *path;

	if (is_builtin(cmd_name))
		exec_builtin(cmd_lst, envp);
	else
	{
		path = get_path(cmd_name);
		printf("Path: %s\n", path);
		execve(path, cmd_lst, envp);
		perror("Error al ejecutar execvp");
    	exit(EXIT_FAILURE);
	}

}
/* void mult_cmds(char **cmds, char **envp)
{
    char *args[MAX_ARGS];
    char *redirs[MAX_REDIRS];
    int num_comandos;
    int i;

    // Separar la línea en comandos por '|'	
    num_comandos = tokenize(cmds[0], "|", cmds, MAX_CMDS);
    if (num_comandos == 0)
        return;

    // Inicializar array de redirecciones
    for (i = 0; i < MAX_REDIRS; i++)
        redirs[i] = NULL;

    // Procesar cada comando individual
    for (i = 0; cmds[i]; i++)
    {
        // Dividir cada comando en argumentos
        tokenize(cmds[i], " \t\n", args, MAX_ARGS);

        // Analizar si hay redirecciones en este comando (>, <, 2>)
        process_redirs(args, redirs);

        // Si es el primer comando y hay redirección de entrada (<)
        if (i == 0 && redirs[0])
            redir_in(redirs[0]);

        // Si no es el último comando: conectar con pipe
        if (i != num_comandos - 1)
            exec_pipe(args[1], args, envp, redirs[2]); // redirs[2] → stderr
        else
            // Último comando: redirige la salida al archivo si hay
            redir_out(args[0], args, envp, redirs[1], redirs[2]); // redirs[1] → stdout, redirs[2] → stderr
    }
} */

/* void exec_line(char *line, char **envp)
{
	char *cmds[MAX_CMDS];
	char *args[MAX_ARGS];
	char *redirs[MAX_REDIRS];
	int num_comandos;
	

	num_comandos = tokenize(line, "|", cmds, MAX_CMDS);
	if (num_comandos == 0)
		return;
	
	printf("Número de comandos: %d\n", num_comandos);
	if (num_comandos == 1) // SOLO UN COMANDO
	{
		tokenize(cmds[0], " \t\n", args, MAX_ARGS);
		process_redirs(args, redirs);

		if (is_builtin(args[0]))
		{
			if (redirs[0]) redir_in(redirs[0]);       // < input
			if (redirs[1]) redir_out_builtin(redirs[1], STDOUT_FILENO);
			if (redirs[2]) redir_out_builtin(redirs[2], STDERR_FILENO);
			exec_builtin(args, envp);
			return;
		}
	}
	mult_cmds(cmds, envp); // Múltiples comandos, o no es builtin
} */

/*  void exec_line(char *line, char **envp)
{
	t_command_line	cmd_line;

	(void)envp;  // Evitar warning por envp no usado
	parse_line(&cmd_line, line);
} */ 
