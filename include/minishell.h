/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:44:43 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/07/08 17:44:43 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/******************************************************************************
 *  																		  *
 *                                 Libraries								  *
 *																			  *
 ******************************************************************************/

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <stdbool.h>

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

# define BANNER ("\n"GREEN BOLD"\
███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n\
████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n\
██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n\
██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n\
██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n\
╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\
"RESET MAGENTA"\t\t     By dediaz-f & jsagaro-\n\n"RESET)

# define BUFF_SIZE 1250

/******************************************************************************
 *  																		  *
 *                                 Structures								  *
 *																			  *
 ******************************************************************************/

typedef struct	s_command_line t_command_line;
typedef struct	s_command t_command;

typedef struct	s_token
{
	char	*token_str;
	bool	quoted;
} t_token;

typedef struct	s_lexer_handler
{
	char *buffer;
	char *cmd_str;
	int buf_len;
	int	buffer_size;
	int	argc; // Number of arguments
	int	n_tokens;
	t_command	*cmd;
	t_token *tokens;
} t_lexer_handler;

typedef struct	s_redirections
{
	int				n_redirs;
	char			**redirs;
} t_redirections;

typedef struct	s_command
{
	char	**args; // Array of command arguments
	char	*cmd_str;
	bool	builtin; // Flag for background execution
	t_redirections	stdin;
	t_redirections	stdout;
	t_redirections	stderr;
	t_redirections	append;
	t_redirections	heredoc;
	t_command_line	*cmd_line;
} t_command;

typedef struct	s_command_line
{
	char		*line;
	char		*err_msg;	
	int			n_cmds;
	bool		execute;
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
void	set_signals(int mode);
void	sigint_handler_in_process(int sig);
void	sigquit_handler_in_process(int sig);
void	disable_echoctl();
void	restore_terminal();

// PIPE & REDIRS
void	exec_pipe(t_command *cmd, char **envp);
void	exec_last(t_command *cmd, char **envp);
void	redirs(t_command *cmd);

// HEREDOC
void	heredoc(t_command *cmd);
void	read_from_stdin(int pipe_fd[2], char  *delim);

// BUILT INS
int		exec_echo(char **args);
int		exec_pwd(void);
int		exec_env(char **envp);
int 	exec_cd(char **args);
int		exec_exit(void);
int 	env_unset(char **argv, char **envp);
int 	env_export(char **argv, char **envp);

// EXEC BUILT INS
int		is_builtin(char *command);
int		exec_builtin(char **args, char **envp);

// SAFE FUNCTIONS
void	*safe_malloc(size_t size, bool calloc_flag);
void	safe_getcwd(char *buf, size_t size);
int		safe_open(const char *path, t_open_flags flags);
void	safe_chdir(const char *path);
void	safe_close(int fd);
void	safe_dup2(int oldfd, int newfd);
int		safe_dup(int fd);

// UTILS
void	print_all(char **args);
void	free_cmd_line(t_command_line *cmd_line);

#endif