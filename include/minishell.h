#ifndef MINISHELL_H
# define MINISHELL_H

/******************************************************************************
 *  																		  *
 *                                 Libraries								  *
 *																			  *
 ******************************************************************************/

#include "../libft/libft.h"
//readline
#include <readline/readline.h>
#include <readline/history.h>
//other libraries
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

/******************************************************************************
 *  																		  *
 *                                 Definitions								  *
 *																			  *
 ******************************************************************************/

# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define RESET "\033[0m"
# define BOLD "\033[1m"
# define UNDERLINE "\033[4m"

# define BUFFER_SIZE 1024
# define MAX_CMDS 25
# define MAX_ARGS 25
# define MAX_REDIRS 3

/******************************************************************************
 *  																		  *
 *                                 Structures								  *
 *																			  *
 ******************************************************************************/

typedef struct s_command
{
	char	**args; // Array of command arguments
	char	*stdin_file;
	char	*stdout_file;
	char	*stderr_file;
	char	*append_file;
	char	*heredoc_delim; // File for heredoc input
	bool	builtin; // Flag for background execution

} t_command;

typedef struct s_command_line
{
	char		*line;
	int			n_cmds;
	t_command	*cmds;
} t_command_line;

typedef struct s_stdfd
{
	int saved_stdin;
	int saved_stdout;
	int saved_stderr;
} t_stdfd;

typedef enum e_open_flags
{
	READ,
	WRITE,
	APPEND,
}	t_open_flags;

typedef enum e_mode
{
	MODE_SHELL,
	MODE_CHILD,
	MODE_HEREDOC
}	t_mode;


/******************************************************************************
 *  																		  *
 *                                 Functions								  *
 *																			  *
 ******************************************************************************/

// EXEC
char	*find_path(char **envp);
char	*try_executable_path(char **paths, char *command);
char	*get_path(char *line);
void	free_args(char **args);
void	exec_line(char *line, char **envp);
void	exec(char *cmd_name, char **cmd_lst, char **envp);

// PARSE
void	parse_line(t_command_line *cmd_line, char *line);

// SIGNALS
void	sigint_handler(int sig);
void    set_signals(int mode);
void	sigint_handler_in_process(int sig);
void	sigquit_handler_in_process(int sig);
void	disable_echoctl();
void	restore_terminal();

// PIPE & REDIRS
void	redir_in(char *file_in);
void	exec_pipe(char *cmd_name, char **cmd_lst, char **envp, char *stderr_file);
void	redir_out(char *cmd_name, char **cmd_lst, char **envp, char *stdout_file, char *stderr_file);

// BUILT INS
int	exec_echo(char **args, char **envp);
int	echo_var(char **argv, char **envp);
int	exec_pwd(void);
int	exec_env(char **envp);
int exec_cd(char **args);
int	exec_exit(void);
int env_unset(char **argv, char **envp);
int env_export(char **argv, char **envp);
char *get_env_value(const char *key, char **envp);

// EXEC BUILT INS
int	is_builtin(char *command);
int	exec_builtin(char **args, char **envp);
/* char *get_path(char **envp, char *line); */

// SAFE FUNCTIONS
void	*safe_malloc(size_t size, bool calloc_flag);
void	safe_getcwd(char *buf, size_t size);
int		safe_open(const char *path, t_open_flags flags);
void	safe_chdir(const char *path);
void	safe_close(int fd);
void	*safe_realloc(void *ptr, size_t old_size, size_t new_size);
void	safe_dup2(int oldfd, int newfd);
int		safe_dup(int fd);

// UTILS
void	banner(void);
void	process_redirs(char **args, char **redir);
int		tokenize(char *linea, char *delim, char **tokens, int max_tokens);
void	print_all(char **args);

#endif