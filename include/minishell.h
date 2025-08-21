/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dediaz-f <dediaz-f@student.42.fr>          +#+  +:+       +#+        */
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
# include <termios.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

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
# define ECHOCTL 0001000

/******************************************************************************
 *  																		  *
 *                                 Structures								  *
 *																			  *
 ******************************************************************************/

//extern sig_atomic_t g_interrupted;
typedef struct s_command_line		t_command_line;
typedef struct s_command			t_command;
typedef struct s_last_exit_status	t_last_status;
//extern t_last_status				g_last_exit_status;




// ----- TIPOS BASE -----
typedef struct s_last_exit_status t_last_status;
typedef struct s_stdfd t_stdfd;
typedef struct s_token t_token;
typedef struct s_token_state t_token_state;

// ----- TIPOS AUXILIARES -----
struct s_stdfd
{
    int saved_stdin;
    int saved_stdout;
    int saved_stderr;
};

struct s_last_exit_status
{
    int status;
    int last_exit_code;
    bool exit_called;
};

struct s_token
{
    char *token_str;
    bool quoted;
};

struct s_token_state
{
    int i;
    int count;
    bool in_sq;
    bool in_dq;
    bool in_token;
};

// ----- TIPOS PRINCIPALES -----
typedef struct s_redirections
{
    int n_redirs;
    char **redirs;
} t_redirections;

// Declaraciones adelantadas para referencias cruzadas
typedef struct s_command_line t_command_line;
typedef struct s_command t_command;

// ----- SHELL -----
typedef struct s_shell
{
    char cwd[BUFFER_SIZE];
    char *line;
    char *prompt;
    char **env;
    t_stdfd stdfd;
    t_last_status last_status;
} t_shell;

// ----- COMMAND -----
struct s_command
{
    char            **args;
    char            *cmd_str;
    bool            builtin;

    t_redirections  stdin;
    t_redirections  stdout;
    t_redirections  stderr;
    t_redirections  append;
    t_shell         *shell;        // referencia al shell para expansiones/env
    t_redirections  heredoc;       // delimitadores heredoc de ESTE comando
    int             heredoc_fd;    // fd resultante (último heredoc válido)
    t_command_line  *cmd_line;     // referencia opcional al padre
};

// ----- COMMAND LINE -----
struct s_command_line
{
    char       *line;
    char       *err_msg;
    int         n_cmds;
    bool        execute;
    t_command  *cmds;
};

// ----- LEXER -----
typedef struct s_lexer_handler
{
    int       n_tokens;
    int       buf_len;
    int       buffer_size;
    int       argc;
    char     *buffer;
    char     *cmd_str;
    t_command *cmd;
    t_token  *tokens;
} t_lexer;

// ----- ENUMS -----
typedef enum e_open_flags
{
    READ,
    WRITE,
    APPEND,
} t_open_flags;

typedef enum e_mode
{
    MODE_SHELL,
    MODE_CHILD,
    MODE_PIPE,
    MODE_HEREDOC
} t_mode;

// ----- LOGICAL OPERATORS -----
typedef struct s_cmd_parts
{
    char *cmd;       // Comando a ejecutar
    int operator;    // 0: ninguno, 1: pipe, 2: &&, 3: ||
} t_cmd_parts;


/******************************************************************************
 *  																		  *
 *                                 Functions								  *
 *																			  *
 ******************************************************************************/

// EXEC
//char	*find_path(char **envp);
char	*try_executable_path(char **paths, char *command);
char	*get_path(char *line, t_shell *shell);
void exec(char *cmd_name, char **cmd_args, t_shell *shell);
void update_last_exit_status(t_shell *shell, int new_status);
void expand_exit_status(char **args, t_shell *shell);
void	get_cmd_info(t_command_line *cmd_line, t_command *cmd,
		char *cmd_str, t_shell *shell);

// PARSE
void parse_line(t_command_line *cmd_line, t_shell *shell, char *line);
void	push_buffer(t_lexer *handler, bool quoted);
void handle_var(t_lexer *handler, char **s, t_shell *shell);
void lexer(t_lexer *handler, t_command *cmd, char *cmd_str, t_shell *shell);
void	get_arguments(t_command *cmd, t_lexer handler);
char	**get_redirec(t_lexer handler, char *redir, int len, int n);
void	get_redirecs(t_command *cmd, t_lexer handler, char *cmd_str);
void	free_handler(t_lexer *handler);
 void get_cmds_info(t_command_line *cmd_line, t_shell *shell, char *line);
void	init_handler(t_lexer *handler, t_command *cmd, char *cmd_str);
bool	is_meta(char *str);
bool	is_file(t_command *cmd, char *str);
int		count_argv(t_command *cmd, t_lexer handler);
char	**split_pipes(char *line, int n_cmds);
int		count_cmds(char *line);
//void run_child(t_command *cmd, t_shell *shell, int in_fd, int out_fd);
void  expand_env_vars(char **args, t_shell *shell);

// SIGNALS
void	sigint_handler(int sig);
void	set_signals(int mode);
//void	sigint_heredoc_handler(int sig);
/* void	sigint_handler_in_process(int sig);
void	sigquit_handler_in_process(int sig); */
void	disable_echoctl(void);
void	restore_terminal(void);
void set_exit_status_direct(t_shell *shell, int code);

// PIPE & REDIRS
void exec_last(t_command *cmd, t_shell *shell);
void exec_line(char *line, t_shell *shell);
void exec_pipe(t_command *cmd, t_shell *shell);
// Pipe helpers (split for norm limits)
int  normalize_wait_status(int status);
void child_exec_command(t_command *cmd, t_shell *shell);
void parent_wait_and_finalize(t_shell *shell, pid_t pid);
void child_exec_pipe(t_command *cmd, t_shell *shell, int pipe_fd[2]);
void parent_setup_pipe_and_wait(t_shell *shell, int pipe_fd[2], pid_t pid);

// REDIRS
void	redirs(t_command *cmd);
void	search_last_redir(t_redirections red, char *cmd_str, int *iter);
void	open_all_files(t_redirections red, t_open_flags flags);

// HEREDOC
void	heredoc(t_command *cmd);
pid_t   start_heredoc_child(int pipe_fd[2], const char *delim);
typedef struct s_heredoc_ctx
{
    int   pipe_fd[2];
    int  *plast_fd;
    bool  is_last;
}   t_heredoc_ctx;
bool    handle_parent_after_child(t_command *cmd, pid_t pid, t_heredoc_ctx *ctx);
void    attach_last_heredoc_to_stdin(int last_fd);
//void	read_from_stdin(int pipe_fd[2], char *delim);
//void sigint_handler_in_heredoc(int sig);
void	read_from_stdin(int pipe_fd[2], char *delim);
void attach_last_heredoc_to_stdin(int last_fd);

// BUILT INS
int     exec_echo(char **args, t_shell *shell);
int		exec_pwd(void);
int     exec_env(t_shell *shell);
int		exec_cd(char **args, t_shell *shell);
int		exec_exit(char **args);
int		env_unset(char **argv, t_shell *shell);
int		env_export(char **argv, t_shell *shell);
int	    update_env_var(char *name, char *new_var, t_shell *shell);
void	add_env_var(char *new_var, t_shell *shell);
void	add_or_update_env(char *name, char *value, t_shell *shell);
bool	is_valid_identifier(const char *s);



// EXEC BUILT INS
int		is_builtin(char *command);
int exec_builtin(char **args, t_shell *shell);	

// SAFE FUNCTIONS
void	*safe_malloc(size_t size, bool calloc_flag);
void	safe_getcwd(char *buf, size_t size);

// LOGICAL OPERATORS
int     count_cmds_with_logical_ops(char *line);
t_cmd_parts *split_cmds_with_logical_ops(char *line, int n_cmds);
void    process_logical_line(char *line, t_shell *shell);
bool    contains_logical_operators(char *line);
int		safe_open(const char *path, t_open_flags flags);
void	safe_chdir(const char *path);
void	safe_close(int fd);
void	safe_dup2(int oldfd, int newfd);
int		safe_dup(int fd);
/* void	free_env(char **env); */
// ENV/FD/MISC (split from main.c)
char    **copy_envp(char **envp);
void    cleanup_shell(t_shell *shell);
void	setup_shell(t_shell *shell, char **envp);
void    save_fds(t_stdfd *std);
void    restore_fds(t_stdfd *std);
void    minishell(t_shell *shell);
char	*get_path(char *line, t_shell *shell);
char	*try_executable_path(char **paths, char *line);

// UTILS
// void	print_all(char **args);
void	free_cmd_line(t_command_line *cmd_line);
char	*get_env(t_shell *shell, const char *name);

#endif
