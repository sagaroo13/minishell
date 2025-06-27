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

void exec(char *cmd_name, char **cmd_lst, char **envp)
{
	char *path;

	if (is_builtin(cmd_name))
		exec_builtin(cmd_lst, envp);
	else
	{
		path = get_path(cmd_name);
		// printf("Path: %s\n", path);
		execve(path, cmd_lst, envp);
		perror("Error al ejecutar execvp");
    	exit(EXIT_FAILURE);
	}

}

 void exec_line(char *line, char **envp)
 {
 	char *cmds[MAX_CMDS];
	char *args[MAX_ARGS];
	char *redirs[MAX_REDIRS];
	int num_comandos;
	int i;
 	num_comandos = tokenize(line, "|", cmds, MAX_CMDS); 	
	print_all(cmds);
 	i = -1;
	while (cmds[++i])
 	{
 		tokenize(cmds[i], " \t\n", args, MAX_ARGS);
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

/*  void exec_line(char *line, char **envp)
{
	t_command_line	cmd_line;

	(void)envp;  // Evitar warning por envp no usado
	parse_line(&cmd_line, line);
} */ 
