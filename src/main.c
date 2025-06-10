#include "../include/minishell.h"

void	minishell(char **envp)
{
	char *line;
	// char **args;
	char cwd[BUFFER_SIZE];
	using_history();
	
	signals();
	while (true)
	{
		line = NULL;
		safe_getcwd(cwd, sizeof(cwd));
		printf(CYAN""BOLD"%s$>"RESET, cwd);
		line = readline(" ");
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
