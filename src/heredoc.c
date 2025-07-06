#include "../include/minishell.h"

void	read_from_stdin(int pipe_fd[2], char  *delim)
{
	char	*line;
	char	*new_line;
	
	close(pipe_fd[0]);
	line = get_next_line(0);
	while (line)
	{
		new_line = ft_strchr(line, '\n');
		*new_line = '\0';
		if (!ft_strcmp(delim, line))
		{
			free(line);
			get_next_line(-1);
			exit(EXIT_SUCCESS);
		}
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
		line = get_next_line(0);
	}
}

void	heredoc(t_command *cmd)
{
	int	pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	if ((pid = fork()) == -1)
		exit(EXIT_FAILURE);
	if (!pid)
		read_from_stdin(pipe_fd, cmd->heredoc_delim);
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		wait(NULL);
	}
}
