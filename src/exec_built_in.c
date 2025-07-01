#include "../include/minishell.h"

int is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strncmp(command, "cd", 3) == 0 ||
		ft_strncmp(command, "echo", 5) == 0 ||
		ft_strncmp(command, "pwd", 4) == 0 ||
		ft_strncmp(command, "exit", 5) == 0 ||
		ft_strncmp(command, "env", 4) == 0 ||
		ft_strncmp(command, "export", 7) == 0 ||
		ft_strncmp(command, "unset", 6) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **args, char **envp)
{
	if (!args || !args[0])
		return (0);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (exec_echo(args, envp));
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (exec_pwd());
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		return (exec_exit());
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		return (exec_cd(args));
	else if (ft_strncmp(args[0], "env", 4) == 0)
		return (exec_env(envp));
	else if (ft_strncmp(args[0], "export", 6) == 0)
	{
		printf("Is in this export\n");
		return (env_export(args, envp));
	}
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		return (env_unset(args, envp));
	return (0);
}
