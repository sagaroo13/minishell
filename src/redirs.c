#include "../include/minishell.h"

void	redirs(t_command *cmd)
{
	int	fd;

	if (cmd->stdin_file)
	{
		fd = safe_open(cmd->stdin_file, READ);
		safe_dup2(fd, STDIN_FILENO);
    	safe_close(fd);
	}
	if (cmd->stdout_file)
	{
		fd = safe_open(cmd->stdout_file, WRITE);
		safe_dup2(fd, STDOUT_FILENO);
    	safe_close(fd);
	}
	if (cmd->stderr_file)
    {
        fd = safe_open(cmd->stderr_file, WRITE);
		safe_dup2(fd, STDERR_FILENO);
		safe_close(fd);
    }
	if (cmd->append_file)
    {
        fd = safe_open(cmd->append_file, APPEND);
		safe_dup2(fd, STDOUT_FILENO);
		safe_close(fd);
    }
}
