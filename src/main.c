
#include "../include/minishell.h"


// main.c
t_last_exit_status g_last_exit_status = {0, 0, false}; 


void	save_fds(t_stdfd *std)
{
	std->saved_stdin = safe_dup(STDIN_FILENO);
	std->saved_stdout = safe_dup(STDOUT_FILENO);
	std->saved_stderr = safe_dup(STDERR_FILENO);
}

void	restore_fds(t_stdfd *std)
{
	safe_dup2(std->saved_stdin, STDIN_FILENO);
	safe_dup2(std->saved_stdout, STDOUT_FILENO);
	safe_dup2(std->saved_stderr, STDERR_FILENO);
	close(std->saved_stdin);
	close(std->saved_stdout);
	close(std->saved_stderr);
}

void	minishell(char **envp)
{
	char	*line;
	char	*prompt;
	t_stdfd	stdfd;
	char	cwd[BUFFER_SIZE];

	disable_echoctl();
	using_history();
	while (true)
	{
		safe_getcwd(cwd, sizeof(cwd));
		prompt = ft_strjoin(cwd, "$> ");
		save_fds(&stdfd);
		line = readline(prompt);
		free(prompt);
		if (!line)
			break ;
		else
		{
			set_signals(MODE_SHELL);
			add_history(line);
			exec_line(line, envp);
		}
		free(line);
		restore_fds(&stdfd);
	}
	restore_terminal();
	clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	printf(BANNER);
	minishell(envp);
	return (EXIT_SUCCESS);
}