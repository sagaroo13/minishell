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

void exec(char *cmd_name, char **cmd_args, char **envp)
{
	char *path;

	if (is_builtin(cmd_name))
		exec_builtin(cmd_args, envp);
	else
	{
		path = get_path(cmd_name);
		// printf("Path: %s\n", path);
		execve(path, cmd_args, envp);
		perror("Error al ejecutar execvp");
    	exit(EXIT_FAILURE);
	}

}
/*  void mult_cmds(char **cmds, char **envp)
{
    char *args[MAX_ARGS];
    char *redirs[MAX_REDIRS];
    int num_comandos;
    int i;
}  */
/* 
void exec_line(char *line, char **envp)
{
	t_command_line	cmd_line;
	int	i;

	parse_line(&cmd_line, line);
	i = -1;
	while (++i < cmd_line.n_cmds)
	{
        if (i != cmd_line.n_cmds - 1)
            exec_pipe(&cmd_line.cmds[i], envp);
        else
            exec_last(&cmd_line.cmds[i], envp);
	}
	free_cmd_line(&cmd_line);
}
 */